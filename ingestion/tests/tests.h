#ifndef TESTS_H
#define TESTS_H
#include "ingestion.h"
#include "unity.h"

V test_ReadEnvVars(V);
V test_HmacVerifyNormal(V);
V test_HmacVerifyErroneous(V);
V test_CheckIdempotency(V);
V test_ValidateJsonObj(V);
V test_ReadRabbitConfig(V);
#endif