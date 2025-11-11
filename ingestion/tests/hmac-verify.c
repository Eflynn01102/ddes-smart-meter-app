#include "tests.h"

V test_HmacVerifyNormal(V) {
    cJSON* MsgJson = {0};
    S8 JsonInput[] = "{\"clientId\":\"12345\",\"currentReading\":\"11\",\"unix\":\"1759865465\",\"fwVersion\":\"1.0.0\",\"unit\":\"kWh\",\"signature\":\"0a75b462339f5b17a2818815cd66cb0cf62d124ec930baf8fdb07c9d360b1ec0\"}";
    MsgJson = cJSON_Parse(JsonInput);
    TEST_ASSERT_EQUAL_CHAR(OK, HmacVerify(MsgJson));
}

V test_HmacVerifyErroneous(V) {
    cJSON* MsgJson = {0};
    S8 JsonInput[] = "{\"clientId\":\"12345\",\"currentReading\":\"11\",\"unix\":\"1759865465\",\"fwVersion\":\"1.0.0\",\"unit\":\"kWh\",\"signature\":\"0a75b462339f5b17a2818815cd66cb0cf62d124ec9s0baf8fdb07c9d360b1ec0\"}";
    MsgJson = cJSON_Parse(JsonInput);
    TEST_ASSERT_EQUAL_CHAR(NOK, HmacVerify(MsgJson));
}