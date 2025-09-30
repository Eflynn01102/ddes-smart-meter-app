#ifndef INGESTION_H
#define INGESTION_H

#include <stdio.h>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <signal.h>
#include "types.h"


#define RABBITMQ_HOST "localhost"
#define RABBITMQ_PORT 5672
#define AMQP_INGESTION_QUEUE_NAME "ingestion.q"
#define TRUE 1
#define FALSE 0
#define OK 0

static V SigIntHandler(S32 SigVal);
volatile static S8 SigIntReceived;

#endif