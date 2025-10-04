#include "ingestion.h"

U8 ReadRabbitConfig(U8* IP, S32* Port, U8* Username, U8* Password) {
    FILE* FP;
    char* TokPtr;
    U8 Conf[1024] = {0};
    U8* Tok = {0};
    U8 ValidTokens = 0;

    FP = NULL;
    FP = fopen("ingestion.conf", "rb");
    if (FP == NULL) {
        fprintf(stdout, "could not open config file\n");
        return NOK;
    }
    
    if (fread(Conf, sizeof(U8), sizeof(Conf)/sizeof(U8), FP) > 0 && ferror(FP) != OK) {
        fprintf(stdout, "could not read config file\n");
        return NOK;
    }

    fclose(FP);

    Tok = NULL;
    Tok = strtok_r(Conf, "\n", &TokPtr);

    while (Tok != NULL) {
        if (Tok[0] == '#' || strcmp(Tok, "") == 0) { //current line is a comment or empty
            Tok = strtok_r(NULL, "\n", &TokPtr);
            continue;
        } else if (ValidTokens == 0) { //this line is the IP address
            sprintf(IP, "%s", Tok);
            ValidTokens++;
        } else if (ValidTokens == 1) { //this line is the port number
            *Port = (S32)atoi(Tok);
            ValidTokens++;
        } else if (ValidTokens == 2) { //this line is the username
            sprintf(Username, "%s", Tok);
            ValidTokens++;
        } else if (ValidTokens == 3) { //this line is the password
            sprintf(Password, "%s", Tok);
            return OK;
        }
        Tok = strtok_r(NULL, "\n", &TokPtr);
    }

    fprintf(stdout, "could not parse config file\n");
    return NOK;
}