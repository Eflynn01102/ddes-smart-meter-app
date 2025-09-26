#include "ingestion.h"

U8 InitiateConnection(AMQP_CONN* Connection) {
    AMQP_SOCK* Socket;

    *Connection = amqp_new_connection();
    
    Socket = NULL;
    Socket = amqp_tcp_socket_new(*Connection);

    if (Socket == FALSE) {
        fprintf(stderr, "Could not create socket");
        return 1;
    }

    {
        U8 Ret;
        Ret = amqp_socket_open(Socket, RABBITMQ_HOST, RABBITMQ_PORT);
        if (Ret != OK) {
            fprintf(stderr, "Could not open tcp socket");
            return 1;
        }
    }   

    {
        enum amqp_response_type_enum_ Ret;
        Ret = amqp_login(*Connection, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest").reply_type;
        if (Ret != AMQP_RESPONSE_NORMAL) {
            fprintf(stderr, "Could not login to amqp");
            return 1;
        }
    }
    
    return OK;
}

V CloseConnection(AMQP_CONN* Connection) {
    amqp_channel_close(*Connection, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(*Connection, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(*Connection);
}

U8 main(U8 argc, U8* argv[]) {
    return 0;
}