#include "ingestion.h"

//globals
volatile sig_atomic_t SigIntReceived;

V SigIntHandler(S32 SigVal) {
    SigIntReceived = TRUE;
}

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
        fprintf(stdout, "Could not create socket\n");
        return NOK;
    }

    {
        S32 Ret;
        Ret = amqp_socket_open(Socket, IP, Port);
        if (Ret != OK) {
            fprintf(stdout, "Could not open tcp socket at %s:%d, %s\n", IP, Port, amqp_error_string2(Ret));
            return NOK;
        }
    }   

    {
        AMQP_RPC_REP Ret;
        Ret = amqp_login(*Connection, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, Username, Password);
        if (Ret.reply_type != AMQP_RESPONSE_NORMAL) {
            fprintf(stdout, "Could not login to amqp\n");
            return NOK;
        }
    }

    amqp_channel_open(*Connection, 1);
    if (CheckRpcReply(*Connection) != OK) {
        fprintf(stdout, "Could not open channel\n");
        return NOK;
    }

    amqp_queue_declare(*Connection, 1, amqp_cstring_bytes(AMQP_INGESTION_QUEUE_NAME), 0, 0, 0, 0, amqp_empty_table);
    if (CheckRpcReply(*Connection) != OK) {
        fprintf(stdout, "Could not declare queue\n");
        return NOK;
    }
        
    amqp_basic_consume(*Connection, 1, amqp_cstring_bytes(AMQP_INGESTION_QUEUE_NAME), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    if (CheckRpcReply(*Connection) != OK) {
        fprintf(stdout, "Could not consume queue\n");
        return NOK;
    }
    
    return OK;
}

V CloseConnection(AMQP_CONN* Connection) {
    amqp_channel_close(*Connection, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(*Connection, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(*Connection);
}

U8 IngestionMainloop(AMQP_CONN* Connection) {
    AMQP_ENVEL Envelope = {0};
    AMQP_RPC_REP Ret;
    struct timeval Timeout = {0};
    cJSON* MsgJson = {0};

    Timeout.tv_sec = 1;

    while (SigIntReceived == FALSE) {
        amqp_maybe_release_buffers(*Connection);

        Ret = amqp_consume_message(*Connection, &Envelope, &Timeout, 0);
        if (Ret.reply_type != AMQP_RESPONSE_NORMAL && Ret.library_error != AMQP_STATUS_TIMEOUT) {
            fprintf(stdout, "Could not consume message, %d\n", Ret.reply_type); 
        } else if (Ret.reply_type == AMQP_RESPONSE_NORMAL) {
            
            //parse JSON
            MsgJson = cJSON_Parse((S8*)Envelope.message.body.bytes);
            if (MsgJson == FALSE) {
                fprintf(stdout, "Could not parse message, %s\n", (S8*)Envelope.message.body.bytes);
            } else if (HmacVerify(MsgJson) != OK) {
                fprintf(stdout, "HMAC verification failed: signature mismatch, %s\n", (S8*)Envelope.message.body.bytes);
            }
            //ToDo check all json tags are present
            //ToDo check data against schema
            else {
                //ToDo process message
                //ToDo publish to events topic
                fprintf(stdout, "successfully processed message processed message from client %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring);
            }
        }
        amqp_destroy_envelope(&Envelope);
    }
    return OK;
}

U8 main(U8 argc, U8* argv[]) {
    U8 IP[32] = {0};
    S32 Port = 0;
    U8 RabbitMQUsername[64] = {0};
    U8 RabbitMQPassword[64] = {0};
    AMQP_CONN Conn = {0};

    SigIntReceived = FALSE;
    if (signal(SIGINT, SigIntHandler) == SIG_ERR) {
        fprintf(stdout, "Could not configure sigint handler\n");
        exit(NOK);
    }

    if (ReadRabbitConfig(IP, &Port, RabbitMQUsername, RabbitMQPassword) != OK) {
        exit(NOK);
    } else {
        fprintf(stdout, "read config\n");
    }

    if (ReadEnvVars() != OK) {
        exit(NOK);
    } else {
        fprintf(stdout, "env vars set\n");
    }

    if (InitiateConnection(&Conn, IP, Port, RabbitMQUsername, RabbitMQPassword) != OK) {
        exit(NOK);
    } else {
        fprintf(stdout, "connected to rabbitmq\n");
    }

    if (IngestionMainloop(&Conn) != OK) {
        exit(NOK);
    } else {
        fprintf(stdout, "SIGINT recieved, mainloop exited\n");
    }
    
    CloseConnection(&Conn);
    fprintf(stdout, "disconnected from rabbitmq\n");
    CleanUpEnvVars();
    fprintf(stdout, "cleaned up env vars\n");
    exit(OK);
}