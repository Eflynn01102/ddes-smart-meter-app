#include "ingestion.h"

U8 ValidateJsonObj(cJSON* MsgJson, S8* ExpectedFwVersion) {
    char* End;
    cJSON *item;

    const char *ExpectedFields[] = {
        "clientId",
        "currentReading",
        "unix",
        "fwVersion",
        "unit",
        "signature"
    };

    for (size_t i = 0; i < sizeof(ExpectedFields)/sizeof(ExpectedFields[0]); ++i) {
        item = cJSON_GetObjectItemCaseSensitive(MsgJson, ExpectedFields[i]);
        if (item == NULL) {
            LogErr("JSON validation failed: message missing json object field: %s\n", ExpectedFields[i]);
            return NOK;
        }
    }

    item = cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId");
    if (!cJSON_IsString(item) || item->valuestring == NULL || item->valuestring[0] == '\0') {
        LogErr("JSON validation failed: message contains empty or non-string clientId\n");
        return NOK;
    }

    item = cJSON_GetObjectItemCaseSensitive(MsgJson, "currentReading");
    if (item->type != cJSON_Number) {
        LogErr("JSON validation failed: currentReading not a number, %d\n");
        return NOK;
    }

    if (item->valuedouble < 0 || item->valuedouble > MAX_READING) {
        LogErr("JSON validation failed: message contains invalid currentReading: %s\n", item->valuestring);
        return NOK;
    }

    item = cJSON_GetObjectItemCaseSensitive(MsgJson, "unix");
    if (item->type != cJSON_Number) {
        LogErr("JSON validation failed: unix not a number\n");
        return NOK;
    }
    
    if (item->valuedouble < 1759863930L || item->valuedouble > 1917626730L) {
        LogErr("JSON validation failed: message contains invalid unix: %s\n", item->valuestring);
        return NOK;
    }

    item = cJSON_GetObjectItemCaseSensitive(MsgJson, "fwVersion");
    if (!cJSON_IsString(item) || item->valuestring == NULL) {
        LogErr("JSON validation failed: fwVersion missing or not a string\n");
        return NOK;
    }
    if (!(ExpectedFwVersion[0] == 0xaa && ExpectedFwVersion[1] == 0xbb &&
          ExpectedFwVersion[2] == 0xcc && ExpectedFwVersion[3] == 0xdd)) {
        if (strcmp(ExpectedFwVersion, item->valuestring) != 0) {
            LogErr("JSON validation failed: message contains invalid fw version: %s (expected %s)\n",
                   item->valuestring, ExpectedFwVersion);
            return NOK;
        }
    }

    item = cJSON_GetObjectItemCaseSensitive(MsgJson, "unit");
    if (!cJSON_IsString(item) || item->valuestring == NULL || strlen(item->valuestring) < 1) {
        LogErr("JSON validation failed: message contains invalid unit: %s\n", item->valuestring ? item->valuestring : "(null)");
        return NOK;
    }

    item = cJSON_GetObjectItemCaseSensitive(MsgJson, "signature");
    if (!cJSON_IsString(item) || item->valuestring == NULL || strlen(item->valuestring) != 64) {
        LogErr("JSON validation failed: message contains invalid signature: %s\n", item->valuestring ? item->valuestring : "(null)");
        return NOK;
    }

    return OK;
}
