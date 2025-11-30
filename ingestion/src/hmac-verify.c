#include "ingestion.h"

//compute a sha256 hash using the secret key on the payload
//of the reading message received and compare it with the signature received

U8 HmacVerify(cJSON* MsgJson) {
    const char *secret = getenv("HMACKEY");
    if (secret == NULL || secret[0] == '\0') {
        LogErr("HMAC verification failed: HMACKEY not set\n");
        return NOK;
    }

    cJSON *cClientId = cJSON_GetObjectItemCaseSensitive(MsgJson, "clientId");
    cJSON *cCurrentReading = cJSON_GetObjectItemCaseSensitive(MsgJson, "currentReading");
    cJSON *cUnix = cJSON_GetObjectItemCaseSensitive(MsgJson, "unix");
    cJSON *cSignature = cJSON_GetObjectItemCaseSensitive(MsgJson, "signature");

    char MsgPayload[256];
    int needed = snprintf(MsgPayload, sizeof(MsgPayload), "%s%d%d",
                          cClientId->valuestring,
                          (S32)cCurrentReading->valuedouble,
                          (S32)cUnix->valuedouble);
    if (needed < 0 || (size_t)needed >= sizeof(MsgPayload)) {
        LogErr("HMAC verification failed: payload too large (%d)\n", needed);
        return NOK;
    }

    unsigned int ResultLen = 0;
    unsigned char *ComputedHmacBytes = HMAC(EVP_sha256(),
                                           (const unsigned char*)secret,
                                           (int)strlen(secret),
                                           (const unsigned char*)MsgPayload,
                                           strlen(MsgPayload),
                                           NULL,
                                           &ResultLen);
    if (ComputedHmacBytes == NULL || ResultLen == 0) {
        LogErr("HMAC computation failed\n");
        return NOK;
    }
    if (ResultLen > 32) {
        LogErr("HMAC unexpected length %u\n", ResultLen);
        return NOK;
    }

    char CalculatedMsgSignature[65];
    for (unsigned int i = 0; i < ResultLen; i++) {
        snprintf(CalculatedMsgSignature + (i * 2), sizeof(CalculatedMsgSignature) - (i * 2),
                 "%02x", ComputedHmacBytes[i]);
    }
    CalculatedMsgSignature[ResultLen * 2] = '\0';

    const char *ReceivedMsgSignature = cSignature->valuestring;
    if (ReceivedMsgSignature == NULL) {
        LogErr("HMAC verification failed: received signature null\n");
        return NOK;
    }

    if (strncmp(ReceivedMsgSignature, CalculatedMsgSignature, 64) == 0) {
        return OK;
    } else {
        LogErr("HMAC mismatch: got %s expected %s\n", ReceivedMsgSignature, CalculatedMsgSignature);
        return NOK;
    }
}
