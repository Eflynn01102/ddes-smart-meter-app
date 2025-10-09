#include "ingestion.h"

//client-id-indexed
MaxReadingType MaxReadings[MAX_CLIENTS] = {0};

U8 CheckIdempotency(cJSON* MsgJson) {
    S32 i = 0;
    S32 Reading = 0;
    char* End;

    //check if this is the largest reading value this client has ever sent
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (strcmp(MaxReadings[i].ClientId, cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring) == OK) {
            Reading = strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "currentReading")->valuestring, &End, 10);
            if (Reading > MaxReadings[i].MaxReading) {
                MaxReadings[i].MaxReading = Reading;
                return OK;
            } else {
                return NOK;
            }
        }
    }

    //if we get here then this is the first reading the client has (successfully) sent
    //add it to the clients arr and its latest value, and retun OK
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (strcmp(MaxReadings[i].ClientId, "") == OK) {
            sprintf(MaxReadings[i].ClientId, "%s", cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId")->valuestring);
            MaxReadings[i].MaxReading = strtol(cJSON_GetObjectItemCaseSensitive(MsgJson, "currentReading")->valuestring, &End, 10);
            return OK;
        }
    }
    return NOK;
}