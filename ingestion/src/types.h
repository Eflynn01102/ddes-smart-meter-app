#ifndef TYPES_H
#define TYPES_H

#include <inttypes.h>

typedef uint8_t U8;
typedef int8_t S8;
typedef uint16_t U16;
typedef int16_t S16;
typedef uint32_t U32;
typedef int32_t S32;
typedef float F32;
typedef double F64;
typedef void V;

typedef amqp_envelope_t AMQP_ENVEL_T;
typedef amqp_rpc_reply_t AMQP_RPC_REP_T;
typedef amqp_connection_state_t AMQP_CONN_T;
typedef amqp_socket_t AMQP_SOCK_T;
typedef amqp_basic_properties_t  AMQP_PROP_T;

#endif