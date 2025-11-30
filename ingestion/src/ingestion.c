#include "ingestion.h"

volatile sig_atomic_t SigTermReceived;

V SigTermHandler(S32 SigVal) {
    LogInfo("sigterm\n");
    SigTermReceived = TRUE;
}

/*U8 IngestionMainloop(AMQP_CONN_T* Connection, ConfigType Conf) {
    AMQP_ENVEL_T Envelope = {0};
    AMQP_RPC_REP_T Ret;
    struct timeval Timeout = { .tv_sec = 1 };
    cJSON* MsgJson = NULL;
    S32 ReconnectDelay = 0;

    while (SigTermReceived == FALSE) {
        Timeout.tv_sec = 1;
        amqp_maybe_release_buffers(*Connection);

        Ret = amqp_consume_message(*Connection, &Envelope, &Timeout, 0);

        if (Ret.reply_type == AMQP_RESPONSE_NONE) {
            continue;
        }

        if (Ret.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION) {
            if (Ret.library_error == AMQP_STATUS_CONNECTION_CLOSED ||
             Ret.library_error == AMQP_STATUS_SOCKET_ERROR ||
             Ret.library_error == AMQP_STATUS_HEARTBEAT_TIMEOUT) {

                LogErr("Connection lost (error %d), reconnecting in %ds...\n", Ret.library_error, ReconnectDelay);

                if (*Connection) {
                    amqp_connection_close(*Connection, AMQP_REPLY_SUCCESS);
                    amqp_destroy_connection(*Connection);
                    *Connection = NULL;
                }

                sleep(ReconnectDelay);
                ReconnectDelay = (ReconnectDelay < 30) ? ReconnectDelay * 2 : 30;

                if (InitiateConnection(Connection, Conf) == OK) {
                    LogInfo("Reconnected successfully to RabbitMQ\n");
                    ReconnectDelay = 1;
                    continue;
                } else {
                    LogErr("Reconnection failed - retrying in %ds\n", ReconnectDelay);
                    continue;
                }
            } else {
                LogErr("RabbitMQ library exception %d\n", Ret.library_error);
            }
        }

        if (Ret.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION) {
            LogErr("RabbitMQ server exception\n");
            continue;
        }
        if (Ret.reply_type == AMQP_RESPONSE_NORMAL) {
            MsgJson = cJSON_ParseWithLength((char*)Envelope.message.body.bytes,
                                            Envelope.message.body.len);
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

            // Success path
            PublishMessageToEventsTopic(Connection, Envelope);
            LogInfo("Processed message from client %s\n",
                    cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring);

            cJSON_Delete(MsgJson);
            MsgJson = NULL;

        ack_and_continue:
            amqp_basic_ack(*Connection, 1, Envelope.delivery_tag, 0);
            amqp_destroy_envelope(&Envelope);
        }
    }

    if (MsgJson) cJSON_Delete(MsgJson);
    return OK;
}*/

U8 IngestionMainloop(AMQP_CONN_T* Connection, ConfigType Conf) {
    AMQP_ENVEL_T Envelope = {0};
    AMQP_RPC_REP_T Ret;
    struct timeval Timeout;
    cJSON* MsgJson = NULL;
    S32 ReconnectDelay = 1;

    LogInfo("Ingestion mainloop started - waiting for messages...\n");

    while (SigTermReceived == FALSE) {
        if (*Connection == NULL) {
            LogWarn("No connection - forcing reconnect in %ds...\n", ReconnectDelay);
            sleep(ReconnectDelay);
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

        Timeout.tv_sec = 1;
        Timeout.tv_usec = 0;

        Ret = amqp_consume_message(*Connection, &Envelope, &Timeout, 0);

        if (Ret.reply_type == AMQP_RESPONSE_NONE) {
            continue;
        }

        if (Ret.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION) {
            LogErr("Library exception (code %d: %s) - forcing reconnect\n",
                   Ret.library_error, amqp_error_string2(Ret.library_error));

            if (*Connection) {
                amqp_connection_close(*Connection, AMQP_REPLY_SUCCESS);
                amqp_destroy_connection(*Connection);
                *Connection = NULL;
            }

            sleep(ReconnectDelay);
            ReconnectDelay = (ReconnectDelay >= 30) ? 30 : ReconnectDelay * 2;

            if (InitiateConnection(Connection, Conf) == OK) {
                LogInfo("Reconnected successfully\n");
                ReconnectDelay = 1;
            } else {
                LogErr("Reconnect failed - retrying in %ds\n", ReconnectDelay);
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
            sleep(5);
            continue;
        }

        if (Ret.reply_type == AMQP_RESPONSE_NORMAL) {
            ReconnectDelay = 1;

            MsgJson = cJSON_ParseWithLength((char*)Envelope.message.body.bytes,
                                            Envelope.message.body.len);
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

            PublishMessageToEventsTopic(Connection, Envelope);
            LogInfo("Processed message from client %s\n",
                    cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring);

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