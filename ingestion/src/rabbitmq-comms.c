#include "ingestion.h"

U8 CheckRpcReply(AMQP_CONN_T Connection) {
    AMQP_RPC_REP_T Ret;
    Ret = amqp_get_rpc_reply(Connection);
    if (Ret.reply_type != AMQP_RESPONSE_NORMAL) {
        return NOK;
    }
    return OK;
}
/*
U8 InitiateConnection(AMQP_CONN_T* Connection, ConfigType Conf) {
    AMQP_SOCK_T* Socket;
    struct timeval Timeout = {0};
    S8 QueueName[1024] = {0};
    S32 QueueI = 0;

    if (*Connection != NULL) {
        amqp_destroy_connection(*Connection);
        *Connection = NULL;
        sleep(1);
    }
    
    *Connection = amqp_new_connection();
    Socket = amqp_tcp_socket_new(*Connection);

    if (!Socket) {
        LogErr("Could not create socket\n");
        amqp_destroy_connection(*Connection);
        *Connection = NULL;
        return NOK;
    }

    {
        S32 Ret;
        Ret = amqp_socket_open(Socket, Conf.RabbitHost, Conf.RabbitPort);
        if (Ret != OK) {
            amqp_destroy_connection(*Connection);
            *Connection = NULL;
            LogErr("Could not open tcp socket at %s:%d, %s\n", Conf.RabbitHost, Conf.RabbitPort, amqp_error_string2(Ret));
            return NOK;
        }
    }   

    {
        AMQP_RPC_REP_T Ret;
        Ret = amqp_login(*Connection, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, Conf.RabbitUsername, Conf.RabbitPassword);
        if (Ret.reply_type != AMQP_RESPONSE_NORMAL) {
            amqp_destroy_connection(*Connection);
            *Connection = NULL;
            LogErr("Could not login to amqp\n");
            return NOK;
        }
    }

    amqp_channel_open(*Connection, 1);
    if (CheckRpcReply(*Connection) != OK) {
        amqp_destroy_connection(*Connection);
        *Connection = NULL;
        LogErr("Could not open channel\n");
        return NOK;
    }

    for (QueueI = 0; QueueI < MAX_CLIENTS; QueueI++) {
        sprintf(QueueName, "%s%d", AMQP_INGESTION_QUEUE_STEM, QueueI);
        amqp_queue_declare(*Connection, 1, amqp_cstring_bytes(QueueName), 0, 0, 0, 0, amqp_empty_table);
        if (CheckRpcReply(*Connection) != OK) {
            amqp_destroy_connection(*Connection);
            *Connection = NULL;
            LogErr("Could not declare billing queue %s\n", QueueName);
            return NOK;
        }

        amqp_basic_consume(*Connection, 1, amqp_cstring_bytes(QueueName), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
        if (CheckRpcReply(*Connection) != OK) {
            LogErr("Could not consume queue %s\n", QueueName);
            amqp_destroy_connection(*Connection);
            *Connection = NULL;
            return NOK;
        }
    }    

    amqp_exchange_declare(*Connection, 1, amqp_cstring_bytes(AMQP_EVENTS_TOPIC_NAME), amqp_cstring_bytes("topic"), 0, 0, 0, 0, amqp_empty_table);
    if (CheckRpcReply(*Connection) != OK) {
        LogErr("Could not declare events topic\n");
        amqp_destroy_connection(*Connection);
            *Connection = NULL;
        return NOK;
    } 
    
    return OK;
}*/

U8 InitiateConnection(AMQP_CONN_T* Connection, ConfigType Conf) {
    AMQP_SOCK_T* Socket = NULL;
    S8 QueueName[1024] = {0};
    S32 QueueI = 0;

    // --- Clean up any existing connection ---
    if (*Connection != NULL) {
        // Drain any pending replies before destruction
        while (amqp_simple_wait_frame(*Connection, NULL) == AMQP_STATUS_OK) {
            // discard frames
        }
        amqp_destroy_connection(*Connection);
        *Connection = NULL;

        // Give RabbitMQ server a short moment to finalize the TCP close
        usleep(200000); // 200 ms
    }

    // --- Create new connection ---
    *Connection = amqp_new_connection();
    if (!*Connection) {
        LogErr("Failed to allocate new AMQP connection\n");
        return NOK;
    }

    Socket = amqp_tcp_socket_new(*Connection);
    if (!Socket) {
        LogErr("Failed to create TCP socket\n");
        amqp_destroy_connection(*Connection);
        *Connection = NULL;
        return NOK;
    }

    int ret = amqp_socket_open(Socket, Conf.RabbitHost, Conf.RabbitPort);
    if (ret != AMQP_STATUS_OK) {
        LogErr("Could not open TCP socket at %s:%d: %s\n",
               Conf.RabbitHost, Conf.RabbitPort, amqp_error_string2(ret));
        amqp_destroy_connection(*Connection);
        *Connection = NULL;
        return NOK;
    }

    // --- Login ---
    amqp_rpc_reply_t login_reply = amqp_login(*Connection, "/",
                                              0, 131072, 0,
                                              AMQP_SASL_METHOD_PLAIN,
                                              Conf.RabbitUsername,
                                              Conf.RabbitPassword);
    if (login_reply.reply_type != AMQP_RESPONSE_NORMAL) {
        LogErr("AMQP login failed\n");
        amqp_destroy_connection(*Connection);
        *Connection = NULL;
        return NOK;
    }

    // --- Open channel ---
    amqp_channel_open(*Connection, 1);
    if (CheckRpcReply(*Connection) != OK) {
        LogErr("Failed to open channel\n");
        amqp_destroy_connection(*Connection);
        *Connection = NULL;
        return NOK;
    }

    // --- Declare and consume queues ---
    for (QueueI = 0; QueueI < MAX_CLIENTS; QueueI++) {
        snprintf(QueueName, sizeof(QueueName), "%s%d", AMQP_INGESTION_QUEUE_STEM, QueueI);

        amqp_queue_declare(*Connection, 1, amqp_cstring_bytes(QueueName),
                           0, 0, 0, 0, amqp_empty_table);
        if (CheckRpcReply(*Connection) != OK) {
            LogErr("Failed to declare queue %s\n", QueueName);
            amqp_destroy_connection(*Connection);
            *Connection = NULL;
            return NOK;
        }

        amqp_basic_consume(*Connection, 1, amqp_cstring_bytes(QueueName),
                           amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
        if (CheckRpcReply(*Connection) != OK) {
            LogErr("Failed to start consuming queue %s\n", QueueName);
            amqp_destroy_connection(*Connection);
            *Connection = NULL;
            return NOK;
        }
    }

    // --- Declare exchange ---
    amqp_exchange_declare(*Connection, 1, amqp_cstring_bytes(AMQP_EVENTS_TOPIC_NAME),
                          amqp_cstring_bytes("topic"),
                          0, 0, 0, 0, amqp_empty_table);
    if (CheckRpcReply(*Connection) != OK) {
        LogErr("Failed to declare exchange %s\n", AMQP_EVENTS_TOPIC_NAME);
        amqp_destroy_connection(*Connection);
        *Connection = NULL;
        return NOK;
    }

    return OK;
}

//no return as its fire and forget
V PublishMessageToEventsTopic(AMQP_CONN_T* Connection, AMQP_ENVEL_T Envelope) {
    AMQP_PROP_T Properties = {0};
    Properties._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    Properties.content_type = amqp_cstring_bytes("text/plain");
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