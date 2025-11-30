#include "ingestion.h"

//client-id-indexed
MaxReadingType MaxReadings[MAX_CLIENTS] = {0};

U8 CheckIdempotency(cJSON* MsgJson) {
    cJSON *cClientId = cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId");
    cJSON *cCurrentReading = cJSON_GetObjectItemCaseSensitive(MsgJson, "currentReading");

    S32 Reading = (S32)cCurrentReading->valuedouble;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (MaxReadings[i].ClientId[0] != '\0' &&
            strncmp(MaxReadings[i].ClientId, cClientId->valuestring, sizeof(MaxReadings[i].ClientId)) == 0) {
            if (Reading > MaxReadings[i].MaxReading) {
                MaxReadings[i].MaxReading = Reading;
                return OK;
            } else {
                return NOK;
            }
        }
    }
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (MaxReadings[i].ClientId[0] == '\0') {
            snprintf(MaxReadings[i].ClientId, sizeof(MaxReadings[i].ClientId), "%s", cClientId->valuestring);
            MaxReadings[i].MaxReading = Reading;
            return OK;
        }
    }
    return NOK;
}
