#include "ingestion.h"

U8 ValidateJsonObj(cJSON* MsgJson) {
    U8 i;
    regex_t Regex;
    S32 RegexResult;
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
            fprintf(stdout, "JSON validation failed: message missing json object field: %s\n", ExpectedFields[i]);
            return NOK;
        }
    }

    //format checks
    //clientId
    if (strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring, &End, 10) < 1) {
        fprintf(stdout, "JSON validation failed: message contains invalid clientId: %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring);
        return NOK;
    }

    //currentReading - note this isn't checking idempotency
    if (strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "currentReading")->valuestring, &End, 10) < 1) {
        fprintf(stdout, "JSON validation failed: message contains invalid currentReading: %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "currentReading")->valuestring);
        return NOK;
    }

    //unix - check between 07/10/2025 and 07/10/2030
    if (strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "unix")->valuestring, &End, 10) < 1759863930 || strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "unix")->valuestring, &End, 10) > 1917626730) {
        fprintf(stdout, "JSON validation failed: message contains invalid unix: %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "unix")->valuestring);
        return NOK;
    }

    //fwVersion
    RegexResult = regcomp(&Regex, "([0-9]+)\\.([0-9]+)\\.([0-9]+)", REG_EXTENDED);
    if (RegexResult != OK) {
        fprintf(stdout, "JSON validation failed: could not configure regex check\n");
        return NOK;
    }

    RegexResult = regexec(&Regex, cJSON_GetObjectItemCaseSensitive(MsgJson, "fwVersion")->valuestring, 0, NULL, 0);
    if (RegexResult != OK) {
        fprintf(stdout, "JSON validation failed: message contains invalid fw version: %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "fwVersion")->valuestring);
        return NOK;
    }
    regfree(&Regex);

    //unit
    if (strlen(cJSON_GetObjectItemCaseSensitive(MsgJson, "unit")->valuestring) < 1) {
        fprintf(stdout, "JSON validation failed: message contains invalid unit: %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "unit")->valuestring);
        return NOK;
    }

    //signature
    if (strlen(cJSON_GetObjectItemCaseSensitive(MsgJson, "signature")->valuestring) != 64) {
        fprintf(stdout, "JSON validation failed: message contains invalid signature: %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "signature")->valuestring);
        return NOK;
    }

    return OK;
}