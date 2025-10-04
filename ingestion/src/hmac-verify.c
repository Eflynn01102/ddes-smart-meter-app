#include "ingestion.h"

//compute a sha256 hash using the secret key on the payload
//of the reading message received and compare it with the signature received

U8 HmacVerify(cJSON* MsgJson) {
    U8* ComputedHmacBytes = {0};
    S8* Result = {0};
    U8* SecretKey = "test";
    S32 ResultLen = 0;
    U8* MsgPayload = (U8*)cJSON_GetObjectItemCaseSensitive(MsgJson, "payload")->valuestring;
    U8* ReceivedMsgSignature = (U8*)cJSON_GetObjectItemCaseSensitive(MsgJson, "signature")->valuestring;
    U8 CalculatedMsgSignature[65] = {0};
    S32 i = 0;
    
    if (MsgPayload == NULL) {
        fprintf(stdout, "HMAC verification failed: payload not found");
        return NOK;
    }
    
    ComputedHmacBytes = HMAC(EVP_sha256(), SecretKey, strlen(SecretKey), MsgPayload, strlen(MsgPayload), NULL, &ResultLen);
    
    for (i = 0; i < ResultLen; i++) {
        sprintf(CalculatedMsgSignature + (i * 2), "%02x", ComputedHmacBytes[i]);
    }

    if (strcmp(ReceivedMsgSignature, CalculatedMsgSignature) == 0) {
        return OK;
    } else {
        fprintf(stdout, "HMAC verification failed: signature mismatch");
        return NOK;
    }
}