#ifndef INGESTION_H
#define INGESTION_H

#define _POSIX_C_SOURCE 200112L

#include <unistd.h>
#include <stdio.h>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <sys/time.h>
#include <stdarg.h>
#include <cjson/cJSON.h>
#include "types.h"

#define AMQP_INGESTION_QUEUE_NAME "ingestion.q"
#define AMQP_EVENTS_TOPIC_NAME "events"
#define AMQP_EVENTS_ROUTING_KEY "billing"
#define TRUE 1
#define FALSE 0
#define OK 0
#define NOK 1
#define strtok_r __strtok_r
#define CLIENT_FW_VER_PATH "package.json"
#define LogInfo(fmt, ...) Log("INFO", fmt, ##__VA_ARGS__)
#define LogErr(fmt, ...)  Log("ERR ", fmt, ##__VA_ARGS__)
#define MAX_CLIENTS 1024
#define MAX_READING 1048576

typedef struct {
    U8 ClientId[1024];
    S32 MaxReading;
} MaxReadingType;

static V SigIntHandler(S32 SigVal);
U8 ReadRabbitConfig(U8* IP, S32* Port, U8* Username, U8* Password);
U8 HmacVerify(cJSON* MsgJson);
U8 ReadEnvVars(V);
U8 CleanUpEnvVars(V);
U8 ValidateJsonObj(cJSON* MsgJson, S8* ExpectedFwVersion);
U8 CheckRpcReply(AMQP_CONN_T Connection);
U8 InitiateConnection(AMQP_CONN_T* Connection, U8* IP, S32 Port, U8* Username, U8* Password);
V CloseConnection(AMQP_CONN_T* Connection);
U8 CheckIdempotency(cJSON* MsgJson);
V PublishMessageToEventsTopic(AMQP_CONN_T* Connection, AMQP_ENVEL_T Envelope);
V Log(S8* Level, S8* fmt, ...);

#endif