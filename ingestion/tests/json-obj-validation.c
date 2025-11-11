#include "tests.h"

V test_ValidateJsonObj(V) {
    U16 Result = 0;
    cJSON* MsgJson = {0};

    //normal
    S8 JsonInput[180] = "{\"clientId\":\"12345\",\"currentReading\":\"11\",\"unix\":\"1759865465\",\"fwVersion\":\"1.0.0\",\"unit\":\"kWh\",\"signature\":\"0a75b462339f5b17a2818815cd66cb0cf62d124ec930baf8fdb07c9d360b1ec0\"}";
    MsgJson = cJSON_Parse(JsonInput);
    Result |= (ValidateJsonObj(MsgJson, getenv("CLIENTFW")) << 0);

    //erroneous on presence check
    sprintf(JsonInput, "{\"clientId\":\"12345\",\"unix\":\"1759865465\",\"fwVersion\":\"1.0.0\",\"unit\":\"kWh\",\"signature\":\"0a75b462339f5b17a2818815cd66cb0cf62d124ec930baf8fdb07c9d360b1ec0\"}");
    MsgJson = cJSON_Parse(JsonInput);
    Result |= (ValidateJsonObj(MsgJson, getenv("CLIENTFW")) << 1);

    //erroneous on clientId length
    sprintf(JsonInput, "{\"clientId\":\"\",\"currentReading\":\"11\",\"unix\":\"1759865465\",\"fwVersion\":\"1.0.0\",\"unit\":\"kWh\",\"signature\":\"0a75b462339f5b17a2818815cd66cb0cf62d124ec930baf8fdb07c9d360b1ec0\"}");
    MsgJson = cJSON_Parse(JsonInput);
    Result |= (ValidateJsonObj(MsgJson, getenv("CLIENTFW")) << 2);

    //erroneous on reading range check
    sprintf(JsonInput, "{\"clientId\":\"12345\",\"currentReading\":\"0\",\"unix\":\"1759865465\",\"fwVersion\":\"1.0.0\",\"unit\":\"kWh\",\"signature\":\"0a75b462339f5b17a2818815cd66cb0cf62d124ec930baf8fdb07c9d360b1ec0\"}");
    MsgJson = cJSON_Parse(JsonInput);
    Result |= (ValidateJsonObj(MsgJson, getenv("CLIENTFW")) << 3);

    sprintf(JsonInput, "{\"clientId\":\"12345\",\"currentReading\":\"1048577\",\"unix\":\"1759865465\",\"fwVersion\":\"1.0.0\",\"unit\":\"kWh\",\"signature\":\"0a75b462339f5b17a2818815cd66cb0cf62d124ec930baf8fdb07c9d360b1ec0\"}");
    MsgJson = cJSON_Parse(JsonInput);
    Result |= (ValidateJsonObj(MsgJson, getenv("CLIENTFW")) << 4);

    //erroneous on unix timestamp range check
    sprintf(JsonInput, "{\"clientId\":\"12345\",\"currentReading\":\"11\",\"unix\":\"13\",\"fwVersion\":\"1.0.0\",\"unit\":\"kWh\",\"signature\":\"0a75b462339f5b17a2818815cd66cb0cf62d124ec930baf8fdb07c9d360b1ec0\"}");
    MsgJson = cJSON_Parse(JsonInput);
    Result |= (ValidateJsonObj(MsgJson, getenv("CLIENTFW")) << 5);

    sprintf(JsonInput, "{\"clientId\":\"12345\",\"currentReading\":\"11\",\"unix\":\"17598654650\",\"fwVersion\":\"1.0.0\",\"unit\":\"kWh\",\"signature\":\"0a75b462339f5b17a2818815cd66cb0cf62d124ec930baf8fdb07c9d360b1ec0\"}");
    MsgJson = cJSON_Parse(JsonInput);
    Result |= (ValidateJsonObj(MsgJson, getenv("CLIENTFW")) << 6);

    //erroneous on fw version mismatch
    sprintf(JsonInput, "{\"clientId\":\"12345\",\"currentReading\":\"11\",\"unix\":\"1759865465\",\"fwVersion\":\"0.9.2\",\"unit\":\"kWh\",\"signature\":\"0a75b462339f5b17a2818815cd66cb0cf62d124ec930baf8fdb07c9d360b1ec0\"}");
    MsgJson = cJSON_Parse(JsonInput);
    Result |= (ValidateJsonObj(MsgJson, getenv("CLIENTFW")) << 7);

    //erroneous on unit length
    sprintf(JsonInput, "{\"clientId\":\"12345\",\"currentReading\":\"11\",\"unix\":\"1759865465\",\"fwVersion\":\"1.0.0\",\"unit\":\"\",\"signature\":\"0a75b462339f5b17a2818815cd66cb0cf62d124ec930baf8fdb07c9d360b1ec0\"}");
    MsgJson = cJSON_Parse(JsonInput);
    Result |= (ValidateJsonObj(MsgJson, getenv("CLIENTFW")) << 8);

    //erroneous on signature length
    sprintf(JsonInput, "{\"clientId\":\"12345\",\"currentReading\":\"11\",\"unix\":\"1759865465\",\"fwVersion\":\"1.0.0\",\"unit\":\"kWh\",\"signature\":\"0a75b46339f5b17a2818815cd66cb0cf62d124ec930baf8fdb07c9d360b1ec0\"}");
    MsgJson = cJSON_Parse(JsonInput);
    Result |= (ValidateJsonObj(MsgJson, getenv("CLIENTFW")) << 9);

    TEST_ASSERT_EQUAL_UINT16(0b1111111110, Result);
}