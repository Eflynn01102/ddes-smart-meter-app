#include "ingestion.h"

U8 CheckRpcReply(AMQP_CONN_T Connection) {
    AMQP_RPC_REP_T Ret;
    Ret = amqp_get_rpc_reply(Connection);
    if (Ret.reply_type != AMQP_RESPONSE_NORMAL) {
        return NOK;
    }
    return OK;
}

U8 InitiateConnection(AMQP_CONN_T* Connection, ConfigType Conf) {
    AMQP_SOCK_T* Socket = NULL;
    S8 QueueName[1024] = {0};
    S32 QueueI = 0;

    if (*Connection != NULL) {
        while (amqp_simple_wait_frame(*Connection, NULL) == AMQP_STATUS_OK);
        amqp_destroy_connection(*Connection);
        *Connection = NULL;
        sleep(1);
    }

    *Connection = amqp_new_connection();
    if (!*Connection) return NOK;

    Socket = amqp_tcp_socket_new(*Connection);
    if (!Socket) {
        amqp_destroy_connection(*Connection);
        *Connection = NULL;
        return NOK;
    }

    int ret = amqp_socket_open(Socket, Conf.RabbitHost, Conf.RabbitPort);
    if (ret != AMQP_STATUS_OK) {
        LogErr("Socket open failed: %s\n", amqp_error_string2(ret));
        amqp_destroy_connection(*Connection);
        *Connection = NULL;
        return NOK;
    }

    amqp_rpc_reply_t r = amqp_login(*Connection, "/", 0, 131072, 60,
                                    AMQP_SASL_METHOD_PLAIN,
                                    Conf.RabbitUsername, Conf.RabbitPassword);
    if (r.reply_type != AMQP_RESPONSE_NORMAL) {
        LogErr("Login failed\n");
        amqp_destroy_connection(*Connection);
        *Connection = NULL;
        return NOK;
    }

    amqp_channel_open(*Connection, 1);
    if (CheckRpcReply(*Connection) != OK) return NOK;

    amqp_basic_qos(*Connection, 1, 0, 1000, 0);
    if (CheckRpcReply(*Connection) != OK) {
        LogErr("Failed to set basic_qos prefetch=1000\n");
        return NOK;
    }

    for (QueueI = 0; QueueI < MAX_CLIENTS; QueueI++) {
        snprintf(QueueName, sizeof(QueueName), "%s%d", AMQP_INGESTION_QUEUE_STEM, QueueI);

        amqp_table_entry_t decl_args[1];
        amqp_table_t decl_table = {1, decl_args};

        decl_args[0].key = amqp_cstring_bytes("x-queue-type");
        decl_args[0].value.kind = AMQP_FIELD_KIND_UTF8;
        decl_args[0].value.value.bytes = amqp_cstring_bytes("stream");

        //amqp_queue_declare(*Connection, 1, amqp_cstring_bytes(QueueName), 0, 1, 0, 0, decl_table);
        //if (CheckRpcReply(*Connection) != OK) {
        //    LogErr("could not set arguments\n");
        //    return NOK;
        //}

        amqp_table_entry_t consume_args[1];

        consume_args[0].key   = amqp_cstring_bytes("x-stream-offset");
        consume_args[0].value.kind = AMQP_FIELD_KIND_UTF8;
        consume_args[0].value.value.bytes = amqp_cstring_bytes("next");

        amqp_table_t consume_table;
        consume_table.num_entries = 1;
        consume_table.entries = consume_args;

        amqp_basic_consume(*Connection, 1,
                        amqp_cstring_bytes(QueueName),
                        amqp_empty_bytes, 0, 0, 0,
                        consume_table);

        if (CheckRpcReply(*Connection) != OK) {
            LogErr("Failed to consume from stream %s\n", QueueName);
            return NOK;
        }
    }

    amqp_exchange_declare(*Connection, 1, amqp_cstring_bytes(AMQP_EVENTS_TOPIC_NAME),
                          amqp_cstring_bytes("topic"), 0, 0, 0, 0, amqp_empty_table);
    if (CheckRpcReply(*Connection) != OK) {
        LogErr("could not declare event topic\n");
        return NOK;
    }
    return OK;
}

//no return as its fire and forget
V PublishMessageToEventsTopic(AMQP_CONN_T* Connection, AMQP_ENVEL_T Envelope) {
    AMQP_PROP_T Properties = {0};
    Properties._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    Properties.content_type = amqp_cstring_bytes("application/json");
    Properties.delivery_mode = 2;

    amqp_basic_publish(*Connection, 1, amqp_cstring_bytes(AMQP_EVENTS_TOPIC_NAME), amqp_cstring_bytes(AMQP_EVENTS_ROUTING_KEY), 0, 0, &Properties, amqp_cstring_bytes((S8*)Envelope.message.body.bytes));
}

V CloseConnection(AMQP_CONN_T* Connection) {
    if (Connection != NULL && *Connection != NULL) {
        amqp_channel_close(*Connection, 1, AMQP_REPLY_SUCCESS);
        amqp_connection_close(*Connection, AMQP_REPLY_SUCCESS);
        amqp_destroy_connection(*Connection);
    }
}