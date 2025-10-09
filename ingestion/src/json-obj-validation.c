#include "ingestion.h"

U8 FetchExpectedFwVersion(S8* FwVersion) {
    FILE* FP = NULL;
    cJSON* JsonObj = {0};
    S8 FileContents[1024] = {0};
    FP = fopen(CLIENT_FW_VER_PATH, "rb");
    if (FP == NULL) {
        LogErr("could not fetch client fw version: cannot access json\n");
        return NOK;
    }
    if (fread(FileContents, sizeof(FileContents[0]), sizeof(FileContents)/sizeof(FileContents[0]), FP) > 0 && ferror(FP) != OK) {
        fclose(FP);
        LogErr("could not fetch client fw version: cannot read json\n");
        return NOK;
    }

    fclose(FP);

    JsonObj = cJSON_Parse(FileContents);
    if (JsonObj == FALSE) {
    LogErr("could not fetch client fw version: cannot parse json\n%s\n", FileContents);
        return NOK;
    }

    if (cJSON_GetObjectItemCaseSensitive(JsonObj, "version") == NULL) {
        cJSON_Delete(JsonObj);
        LogErr("could not fetch client fw version: fw version not present\n");
        return NOK;
    }
    
    if (sprintf(FwVersion, "%s", cJSON_GetObjectItemCaseSensitive(JsonObj, "version")->valuestring) > 0) {
        
        cJSON_Delete(JsonObj);
        return OK;
    }
    LogErr("could not fetch client fw version: cannot write to result var\n");
    cJSON_Delete(JsonObj);
    return NOK;
}

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
    if (strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring, &End, 10) < 1) {
        LogErr("JSON validation failed: message contains invalid clientId: %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring);
        return NOK;
    }

    //currentReading - note this isn't checking idempotency
    if (strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "currentReading")->valuestring, &End, 10) < 1) {
        LogErr("JSON validation failed: message contains invalid currentReading: %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "currentReading")->valuestring);
        return NOK;
    }

    //unix - check between 07/10/2025 and 07/10/2030
    if (strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "unix")->valuestring, &End, 10) < 1759863930 || strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "unix")->valuestring, &End, 10) > 1917626730) {
        LogErr("JSON validation failed: message contains invalid unix: %s\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "unix")->valuestring);
        return NOK;
    }

    //fwVersion
    if (strcmp(ExpectedFwVersion, cJSON_GetObjectItemCaseSensitive(MsgJson, "fwVersion")->valuestring) != OK) {
        LogErr("JSON validation failed: message contains invalid fw version: %s (expected %s)\n", cJSON_GetObjectItemCaseSensitive(MsgJson, "fwVersion")->valuestring, ExpectedFwVersion);
        return NOK;
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