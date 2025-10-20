#include "ingestion.h"

U8 ValidateJsonObj(cJSON* MsgJson, S8* ExpectedFwVersion) {
    U8 i;
    char* End;
    U8 ExpectedFields[6][65] = {
        "clientId",
        "currentReading",
        "unix",
        "fwVersion",
        "unit",
        "signature"
    };
    

    //presence check all fields
    for (i = 0; i < 6; i++) {
        if (cJSON_GetObjectItemCaseSensitive(MsgJson, ExpectedFields[i]) == NULL) {
            LogErr("JSON validation failed: message missing json object field: %s\n", ExpectedFields[i]);
            return NOK;
        }
    }

    //format checks
    //clientId
    if (strcmp(cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring, "") == OK) {
        LogErr("JSON validation failed: message contains empty clientId");
        return NOK;
    }

    //currentReading - note this isn't checking idempotency
    if (strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "currentReading")->valuestring, &End, 10) < 1 || strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "currentReading")->valuestring, &End, 10) > MAX_READING) {
        LogErr("JSON validation failed: message contains invalid currentReading: %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "currentReading")->valuestring);
        return NOK;
    }

    //unix - check between 07/10/2025 and 07/10/2030
    if (strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "unix")->valuestring, &End, 10) < 1759863930 || strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "unix")->valuestring, &End, 10) > 1917626730) {
        LogErr("JSON validation failed: message contains invalid unix: %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "unix")->valuestring);
        return NOK;
    }

    //fwVersion
    if (!(ExpectedFwVersion[0] == 0xaa && ExpectedFwVersion[1] == 0xbb && ExpectedFwVersion[2] == 0xcc && ExpectedFwVersion[3] == 0xdd)) {
        if (strcmp(ExpectedFwVersion, cJSON_GetObjectItemCaseSensitive(MsgJson, "fwVersion")->valuestring) != OK) {
            LogErr("JSON validation failed: message contains invalid fw version: %s (expected %s)\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "fwVersion")->valuestring, ExpectedFwVersion);
            return NOK;
        }
    }

    //unit
    if (strlen(cJSON_GetObjectItemCaseSensitive(MsgJson, "unit")->valuestring) < 1) {
        LogErr("JSON validation failed: message contains invalid unit: %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "unit")->valuestring);
        return NOK;
    }

    //signature
    if (strlen(cJSON_GetObjectItemCaseSensitive(MsgJson, "signature")->valuestring) != 64) {
        LogErr("JSON validation failed: message contains invalid signature: %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "signature")->valuestring);
        return NOK;
    }

    return OK;
}