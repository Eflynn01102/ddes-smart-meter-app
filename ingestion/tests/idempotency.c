#include "tests.h"

V test_CheckIdempotency(V) {
    U8 Result = 0;
    cJSON* MsgJson = {0};
    S8 JsonInput[175] = "{\"clientId\":\"12345\",\"currentReading\":\"11\",\"unix\":\"1759865465\",\"fwVersion\":\"1.0.0\",\"unit\":\"kWh\",\"signature\":\"0a75b462339f5b17a2818815cd66cb0cf62d124ec930baf8fdb07c9d360b1ec0\"}";
    MsgJson = cJSON_Parse(JsonInput);
    Result |= CheckIdempotency(MsgJson); //normal, first reading
    Result |= (CheckIdempotency(MsgJson) << 1); //erroneous, same reading again
    
    sprintf(JsonInput, "{\"clientId\":\"12345\",\"currentReading\":\"12\",\"unix\":\"1759865465\",\"fwVersion\":\"1.0.0\",\"unit\":\"kWh\",\"signature\":\"0a75b462339f5b17a2818815cd66cb0cf62d124ec930baf8fdb07c9d360b1ec0\"}");
    MsgJson = cJSON_Parse(JsonInput);
    Result |= (CheckIdempotency(MsgJson) << 2); //normal, reading incremented (the hmac signature doesn't matter)
    
    TEST_ASSERT_EQUAL_CHAR(0b010, Result);
}