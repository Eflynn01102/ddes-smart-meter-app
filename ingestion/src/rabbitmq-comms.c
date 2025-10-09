#include "ingestion.h"

U8 CheckRpcReply(AMQP_CONN Connection) {
    AMQP_RPC_REP Ret;
    Ret = amqp_get_rpc_reply(Connection);
    if (Ret.reply_type != AMQP_RESPONSE_NORMAL) {
        return NOK;
    }
    return OK;
}

U8 InitiateConnection(AMQP_CONN* Connection, U8* IP, S32 Port, U8* Username, U8* Password) {
    AMQP_SOCK* Socket;

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
            LogErr("Could not open tcp socket at %s:%d, %s\n", IP, Port, amqp_error_string2(Ret));
            return NOK;
        }
    }   

    {
        AMQP_RPC_REP Ret;
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
        LogErr("Could not declare queue\n");
        return NOK;
    }
        
    amqp_basic_consume(*Connection, 1, amqp_cstring_bytes(AMQP_INGESTION_QUEUE_NAME), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    if (CheckRpcReply(*Connection) != OK) {
        LogErr("Could not consume queue\n");
        return NOK;
    }
    
    return OK;
}

V CloseConnection(AMQP_CONN* Connection) {
    amqp_channel_close(*Connection, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(*Connection, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(*Connection);
}