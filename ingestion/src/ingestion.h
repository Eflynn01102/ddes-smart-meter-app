#ifndef INGESTION_H
#define INGESTION_H

#include <stdio.h>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <sys/time.h>
#include "cJSON.h"
#include "types.h"

#define AMQP_INGESTION_QUEUE_NAME "ingestion.q"
#define TRUE 1
#define FALSE 0
#define OK 0
#define NOK 1
#define strtok_r __strtok_r

static V SigIntHandler(S32 SigVal);
U8 ReadRabbitConfig(U8* IP, S32* Port, U8* Username, U8* Password);
U8 HmacVerify(cJSON* MsgJson);

#endif