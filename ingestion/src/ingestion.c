#include "ingestion.h"

volatile sig_atomic_t SigIntReceived;

V SigIntHandler(S32 SigVal) {
    LogInfo("sigint\n");
    SigIntReceived = TRUE;
}

U8 IngestionMainloop(AMQP_CONN_T* Connection) {
    AMQP_ENVEL_T Envelope = {0};
    AMQP_RPC_REP_T Ret;
    struct timeval Timeout = {0};
    cJSON* MsgJson = {0};
    S32 ClientId = 0;
    S32 Reading = 0;

    Timeout.tv_sec = 1;

    while (SigIntReceived == FALSE) {
        amqp_maybe_release_buffers(*Connection);

        Ret = amqp_consume_message(*Connection, &Envelope, &Timeout, 0);
        if (Ret.reply_type != AMQP_RESPONSE_NORMAL && Ret.library_error != AMQP_STATUS_TIMEOUT) {
            LogErr("Could not consume message, %d\n", Ret.reply_type); 
        } else if (Ret.reply_type == AMQP_RESPONSE_NORMAL) {
            
            //parse JSON
            MsgJson = cJSON_Parse((S8*)Envelope.message.body.bytes);
            if (MsgJson == FALSE) {
                LogErr("Could not parse message, %s\n", (S8*)Envelope.message.body.bytes);
            }
            
            if (ValidateJsonObj(MsgJson, getenv("CLIENTFW")) == OK) { //any error messages would be reported within this function hence no error message if != OK
                if (HmacVerify(MsgJson) == OK) {
                    if (CheckIdempotency(MsgJson) == OK) {
                        PublishMessageToEventsTopic(Connection, Envelope);
                        LogInfo("successfully processed message from client %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring);
                    } else {
                        LogInfo("ignoring message from client %s: idempotency check failed\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring);
                    }
                } else {
                    LogErr("HMAC verification failed: signature mismatch, %s\n", (S8*)Envelope.message.body.bytes);
                }
            }
        }
        amqp_destroy_envelope(&Envelope);
    }
    cJSON_Delete(MsgJson);
    return OK;
}

U8 main(U8 argc, U8* argv[]) {
    U8 IP[32] = {0};
    S32 Port = 0;
    U8 RabbitMQUsername[64] = {0};
    U8 RabbitMQPassword[64] = {0};
    AMQP_CONN_T Conn = {0};
    S32 ConnectionAttempt = 0;

    LogInfo("INGESTION (%s %s)\n", __DATE__, __TIME__);

    SigIntReceived = FALSE;
    if (signal(SIGINT, SigIntHandler) == SIG_ERR) {
        LogErr("Could not configure sigint handler\n");
        exit(NOK);
    }

    if (ReadRabbitConfig(IP, &Port, RabbitMQUsername, RabbitMQPassword) != OK) {
        LogErr("could not read rabbitmq config\n");
        exit(NOK);
    } else {
        LogInfo("read rabbitmq config\n");
    }

    if (ReadEnvVars() != OK) {
        LogErr("could not set env vars\n");
        exit(NOK);
    } else {
        LogInfo("env vars set\n");
    }

    do {
        ConnectionAttempt++;
        LogInfo("connecting to rabbitmq, attempt %d\n", ConnectionAttempt);
        InitiateConnection(&Conn, IP, Port, RabbitMQUsername, RabbitMQPassword);
        sleep(5);
    } while (SigIntReceived == FALSE && Conn == NULL);
    
    if (Conn != NULL) {
        LogInfo("connected to rabbitmq\n");
    }

    if (IngestionMainloop(&Conn) != OK) {
        exit(NOK);
    } else {
        LogInfo("SIGINT recieved, mainloop exited\n");
    }
    
    CloseConnection(&Conn);
    LogInfo("disconnected from rabbitmq\n");
    CleanUpEnvVars();
    LogInfo("cleaned up env vars\n");
    exit(OK);
}