#include "ingestion.h"

volatile sig_atomic_t SigTermReceived;

V SigTermHandler(S32 SigVal) {
    LogInfo("sigterm\n");
    SigTermReceived = TRUE;
}

U8 IngestionMainloop(AMQP_CONN_T* Connection, ConfigType Conf) {
    AMQP_ENVEL_T Envelope = {0};
    AMQP_RPC_REP_T Ret;
    struct timeval Timeout;
    cJSON* MsgJson = NULL;
    S32 ReconnectDelay = 1;

    LogInfo("Ingestion mainloop started - waiting for messages...\n");

    while (SigTermReceived == FALSE) {
        if (*Connection == NULL) {
            LogErr("No connection - forcing reconnect in %ds...\n", ReconnectDelay);
            ReconnectDelay = (ReconnectDelay >= 30) ? 30 : ReconnectDelay * 2;
            if (InitiateConnection(Connection, Conf) == OK) {
                LogInfo("Connection established - resuming consume\n");
                ReconnectDelay = 1;
            } else {
                LogErr("Connect failed - retrying in %ds\n", ReconnectDelay);
            }
            continue;
        }

        amqp_maybe_release_buffers(*Connection);

        Timeout.tv_sec = 30;
        Timeout.tv_usec = 0;

        Ret = amqp_consume_message(*Connection, &Envelope, &Timeout, 0);

        if (Ret.reply_type == AMQP_RESPONSE_NONE) {
            continue;
        }

        if (Ret.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION) {
            if (Ret.library_error == AMQP_STATUS_TIMEOUT) {
                if (*Connection) {
                    amqp_connection_close(*Connection, AMQP_REPLY_SUCCESS);
                    amqp_destroy_connection(*Connection);
                    *Connection = NULL;
                }

                ReconnectDelay = (ReconnectDelay >= 30) ? 30 : ReconnectDelay * 2;

                if (InitiateConnection(Connection, Conf) == OK) {
                    ReconnectDelay = 1;
                } else {
                    LogErr("Reconnect failed - retrying in %ds\n", ReconnectDelay);
                }
            } else {
                LogErr("library exception %d\n", Ret.library_error);
            }
            continue;
        }

        if (Ret.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION) {
            LogErr("Server exception - forcing reconnect\n");
            if (*Connection) {
                amqp_connection_close(*Connection, AMQP_REPLY_SUCCESS);
                amqp_destroy_connection(*Connection);
                *Connection = NULL;
            }
            continue;
        }

        if (Ret.reply_type == AMQP_RESPONSE_NORMAL) {
            ReconnectDelay = 1;

            MsgJson = cJSON_ParseWithLength((char*)Envelope.message.body.bytes, Envelope.message.body.len);
            if (!MsgJson) {
                LogErr("JSON parse failed: %.*s\n",
                    (int)Envelope.message.body.len,
                    (char*)Envelope.message.body.bytes);
                goto ack_and_continue;
            }

            if (ValidateJsonObj(MsgJson, getenv("CLIENTFW")) != OK ||
                HmacVerify(MsgJson) != OK ||
                CheckIdempotency(MsgJson) != OK) {
                cJSON_Delete(MsgJson);
                MsgJson = NULL;
                goto ack_and_continue;
            }

            // Transform client message to billing service format
            cJSON *clientIdObj = cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId");
            cJSON *currentReadingObj = cJSON_GetObjectItemCaseSensitive(MsgJson, "currentReading");
            cJSON *unixObj = cJSON_GetObjectItemCaseSensitive(MsgJson, "unix");
            
            const char *clientIdStr = NULL;
            if (cJSON_IsString(clientIdObj) && (clientIdObj->valuestring != NULL)) {
                clientIdStr = clientIdObj->valuestring;
            } else {
                LogErr("Warning: message missing valid clientId field\n");
                goto cleanup_and_ack;
            }

            // Create billing message with transformed format
            cJSON *billingMsg = cJSON_CreateObject();
            if (!billingMsg) {
                LogErr("Failed to create billing message JSON\n");
                goto cleanup_and_ack;
            }

            // Generate unique readingId from clientId and timestamp
            char readingId[256];
            snprintf(readingId, sizeof(readingId), "%s-%ld", clientIdStr, (long)unixObj->valuedouble);
            
            // Convert unix timestamp to ISO-8601 format
            time_t unixTime = (time_t)unixObj->valuedouble;
            struct tm *tm_info = gmtime(&unixTime);
            char timestamp[64];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", tm_info);

            cJSON_AddStringToObject(billingMsg, "readingId", readingId);
            cJSON_AddStringToObject(billingMsg, "accountId", clientIdStr);
            cJSON_AddStringToObject(billingMsg, "meterId", clientIdStr);
            cJSON_AddStringToObject(billingMsg, "timestamp", timestamp);
            cJSON_AddNumberToObject(billingMsg, "cumulativeKwh", currentReadingObj->valuedouble);
            cJSON_AddNumberToObject(billingMsg, "schema", 1);

            char *billingMsgStr = cJSON_PrintUnformatted(billingMsg);
            if (!billingMsgStr) {
                LogErr("Failed to serialize billing message\n");
                cJSON_Delete(billingMsg);
                goto cleanup_and_ack;
            }

            // Create envelope with transformed message
            AMQP_ENVEL_T transformedEnvelope = Envelope;
            transformedEnvelope.message.body = amqp_cstring_bytes(billingMsgStr);
            
            PublishMessageToEventsTopic(Connection, transformedEnvelope);
            
            LogInfo("Processed message from client %s, reading: %.2f kWh\n", 
                    clientIdStr, currentReadingObj->valuedouble);

            cJSON_free(billingMsgStr);
            cJSON_Delete(billingMsg);

            cleanup_and_ack:

            cJSON_Delete(MsgJson);
            MsgJson = NULL;

            ack_and_continue:
                amqp_basic_ack(*Connection, 1, Envelope.delivery_tag, 0);
                amqp_destroy_envelope(&Envelope);
        }
    }

    LogInfo("Ingestion loop exiting (SIGTERM received)\n");
    if (MsgJson) cJSON_Delete(MsgJson);
    return OK;
}

U8 main(U8 argc, U8* argv[]) {
    ConfigType Config = {0};
    AMQP_CONN_T Conn = NULL;
    S32 ConnectionAttempt = 0;

    LogInfo("INGESTION (STREAMS) (%s %s)\n", __DATE__, __TIME__);

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
    
    CloseConnection(&Conn);
    CleanUpEnvVars();
    LogInfo("cleaned up env vars\n");
    exit(OK);
}