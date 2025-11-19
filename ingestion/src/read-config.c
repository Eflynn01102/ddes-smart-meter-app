#include "ingestion.h"

U8 ReadConfig(ConfigType* Conf) {
    FILE* FP;
    char* TokPtr;
    char* SubTokPtr;
    U8 ConfData[1024] = {0};
    char* Tok = {0};
    char* SubTokKey = {0};
    char* SubTokElem = {0};
    U8 Ret = 0;

    FP = NULL;
    FP = fopen("ingestion.conf", "rb");
    if (FP == NULL) {
        LogErr("could not open config file\n");
        return NOK;
    }
    
    size_t BytesRead = fread(ConfData, sizeof(U8), sizeof(ConfData)/sizeof(U8), FP);
    
    if (ferror(FP) != OK) {
        LogErr("could not read config file\n");
        return NOK;
    }

    fclose(FP);

    Tok = NULL;
    Tok = strtok_r(ConfData, "\n", &TokPtr);

    while (Tok != NULL) {
        if (Tok[0] == '#' || strcmp(Tok, "") == 0) { //current line is a comment or empty
            Tok = strtok_r(NULL, "\n", &TokPtr);
            continue;
        } else {
            SubTokKey = strtok_r(Tok, "=", &SubTokPtr);
            SubTokElem = strtok_r(NULL, "=", &SubTokPtr);
            
            if (strcmp(SubTokKey, "rabbitmq_host") == 0) {
                sprintf(Conf->RabbitHost, "%s", SubTokElem);
                Ret |= (1 << 0);
            } else if (strcmp(SubTokKey, "rabbitmq_port") == 0) {
                Conf->RabbitPort = (S32)atoi(SubTokElem);
                Ret |= (1 << 1);
            } else if (strcmp(SubTokKey, "rabbitmq_user") == 0) {
                sprintf(Conf->RabbitUsername, "%s", SubTokElem);
                Ret |= (1 << 2);
            } else if (strcmp(SubTokKey, "rabbitmq_password") == 0) {
                sprintf(Conf->RabbitPassword, "%s", SubTokElem);
                Ret |= (1 << 3);
            } else if (strcmp(SubTokKey, "alerts_host") == 0) {
                sprintf(Conf->AlertsHost, "%s", SubTokElem);
                Ret |= (1 << 4);
            } else if (strcmp(SubTokKey, "alerts_port") == 0) {
                Conf->AlertsPort = (S32)atoi(SubTokElem);
                Ret |= (1 << 5);
            }
        }
        Tok = strtok_r(NULL, "\n", &TokPtr);
    }
    return Ret;
}