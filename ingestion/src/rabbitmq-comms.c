#include "ingestion.h"

U8 CheckRpcReply(AMQP_CONN_T Connection) {
    AMQP_RPC_REP_T Ret;
    Ret = amqp_get_rpc_reply(Connection);
    if (Ret.reply_type != AMQP_RESPONSE_NORMAL) {
        return NOK;
    }
    return OK;
}

U8 InitiateConnection(AMQP_CONN_T* Connection, U8* IP, S32 Port, U8* Username, U8* Password) {
    AMQP_SOCK_T* Socket;

    *Connection = amqp_new_connection();
    
    Socket = NULL;
    Socket = amqp_tcp_socket_new(*Connection);

    if (Socket == FALSE) {
        LogErr("Could not create socket\n");
        return NOK;
    }

    {
        S32 Ret;
        Ret = amqp_socket_open(Socket, IP, Port);
        if (Ret != OK) {
            *Connection = NULL;
            LogErr("Could not open tcp socket at %s:%d, %s\n", IP, Port, amqp_error_string2(Ret));
            return NOK;
        }
    }   

    {
        AMQP_RPC_REP_T Ret;
        Ret = amqp_login(*Connection, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, Username, Password);
        if (Ret.reply_type != AMQP_RESPONSE_NORMAL) {
            LogErr("Could not login to amqp\n");
            return NOK;
        }
    }

    amqp_channel_open(*Connection, 1);
    if (CheckRpcReply(*Connection) != OK) {
        LogErr("Could not open channel\n");
        return NOK;
    }

    amqp_queue_declare(*Connection, 1, amqp_cstring_bytes(AMQP_INGESTION_QUEUE_NAME), 0, 0, 0, 0, amqp_empty_table);
    if (CheckRpcReply(*Connection) != OK) {
        LogErr("Could not declare billing queue\n");
        return NOK;
    }

    amqp_exchange_declare(*Connection, 1, amqp_cstring_bytes(AMQP_EVENTS_TOPIC_NAME), amqp_cstring_bytes("topic"), 0, 0, 0, 0, amqp_empty_table);
    if (CheckRpcReply(*Connection) != OK) {
        LogErr("Could not declare events topic\n");
        return NOK;
    }
        
    amqp_basic_consume(*Connection, 1, amqp_cstring_bytes(AMQP_INGESTION_QUEUE_NAME), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    if (CheckRpcReply(*Connection) != OK) {
        LogErr("Could not consume queue\n");
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
    amqp_channel_close(*Connection, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(*Connection, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(*Connection);
}