#include "ingestion.h"

volatile sig_atomic_t SigTermReceived;

V SigTermHandler(S32 SigVal) {
    LogInfo("sigterm\n");
    SigTermReceived = TRUE;
}

U8 IngestionMainloop(AMQP_CONN_T* Connection, ConfigType Conf) {
    AMQP_ENVEL_T Envelope = {0};
    AMQP_RPC_REP_T Ret;
    struct timeval Timeout = {0};
    cJSON* MsgJson = {0};
    S32 ClientId = 0;
    S32 Reading = 0;

    Timeout.tv_sec = 1;
    while (SigTermReceived == FALSE) {
        amqp_maybe_release_buffers(*Connection);
        Ret = amqp_consume_message(*Connection, &Envelope, &Timeout, 0);
        
        if (Ret.reply_type == AMQP_RESPONSE_NONE) {
            continue;
        } else if (Ret.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION) {
            if (Ret.library_error == AMQP_STATUS_TIMEOUT) {
                continue;
            } else if (Ret.library_error == AMQP_STATUS_CONNECTION_CLOSED) {
                if (InitiateConnection(Connection, Conf) == OK) {
                    LogInfo("reconnected to rabbitmq\n");
                    continue;
                }
            }
            LogErr("library exception\n");
            continue;
        } else if (Ret.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION) {
            LogErr("server exception\n");
            continue;
        } else if (Ret.reply_type == AMQP_RESPONSE_NORMAL) {
            //parse JSON
            MsgJson = cJSON_Parse((S8*)Envelope.message.body.bytes);
            if (MsgJson == FALSE) {
                LogErr("Could not parse message, %s\n", (S8*)Envelope.message.body.bytes);
                Alert("message parse failure, check log", Conf);
            }
            
            if (ValidateJsonObj(MsgJson, getenv("CLIENTFW")) == OK) { //any error messages would be reported within this function hence no error message if != OK
                if (HmacVerify(MsgJson) == OK) {
                    if (CheckIdempotency(MsgJson) == OK) {
                        PublishMessageToEventsTopic(Connection, Envelope);
                        LogInfo("successfully processed message from client %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring);
                    } else {
                        LogInfo("ignoring message from client %s: idempotency check failed\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring);
                        Alert("idempotency check failure, check log", Conf);
                    }
                } else {
                    LogErr("HMAC verification failed: signature mismatch, %s\n", (S8*)Envelope.message.body.bytes);Alert("idempotency check failure, check log", Conf);
                    Alert("hmac verification failure, check log", Conf);
                }
            }
            amqp_destroy_envelope(&Envelope);
        }
    }
    cJSON_Delete(MsgJson);
    return OK;
}

U8 main(U8 argc, U8* argv[]) {
    ConfigType Config = {0};
    AMQP_CONN_T Conn = NULL;
    S32 ConnectionAttempt = 0;

    LogInfo("INGESTION (%s %s)\n", __DATE__, __TIME__);

    SigTermReceived = FALSE;
    if (signal(SIGTERM, SigTermHandler) == SIG_ERR) {
        LogErr("Could not configure sigterm handler\n");
        exit(NOK);
    }

    if (ReadConfig(&Config) != CONFIG_READ_OK) {
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
        LogInfo("contacting rabbitmq, attempt %d\n", ConnectionAttempt);

        if (InitiateConnection(&Conn, Config) == OK) {
            LogInfo("connected to rabbitmq\n");
            break;
        } else {
            LogErr("connection attempt failed\n");
            if (Conn != NULL) {
                amqp_destroy_connection(Conn);
                Conn = NULL;
            }
            sleep(5);
        }
    } while (!SigTermReceived);

    if (Conn != NULL) {
        LogInfo("starting mainloop\n");
        IngestionMainloop(&Conn, Config);
        amqp_destroy_connection(Conn);
    }
    
    CloseConnection(Conn);
    CleanUpEnvVars();
    LogInfo("cleaned up env vars\n");
    exit(OK);
}