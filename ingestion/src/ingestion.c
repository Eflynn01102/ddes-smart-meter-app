#include "ingestion.h"

//globals
volatile sig_atomic_t SigIntReceived;

V SigIntHandler(S32 SigVal) {
    SigIntReceived = TRUE;
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
            }
            
            if (ValidateJsonObj(MsgJson) == OK) { //any error messages would be reported within this function
                if (HmacVerify(MsgJson) == OK) {
                    //ToDo process message
                    //ToDo publish to events topic
                    fprintf(stdout, "successfully processed message processed message from client %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring);
                } else {
                    fprintf(stdout, "HMAC verification failed: signature mismatch, %s\n", (S8*)Envelope.message.body.bytes);
                }
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