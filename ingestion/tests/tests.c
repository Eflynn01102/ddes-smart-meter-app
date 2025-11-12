#include "tests.h"

V setUp(V) {
    ReadEnvVars();
    return ;
}

V tearDown(V) {
    return ;
}

V main(V) {
    UNITY_BEGIN();
    RUN_TEST(test_ReadEnvVars);
    RUN_TEST(test_HmacVerifyNormal);
    RUN_TEST(test_HmacVerifyErroneous);
    RUN_TEST(test_CheckIdempotency);
    RUN_TEST(test_ValidateJsonObj);
    RUN_TEST(test_ReadRabbitConfig);
    RUN_TEST(test_RabbitMQComms);
}