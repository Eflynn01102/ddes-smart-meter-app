#include "ingestion.h"

U8 ReadRabbitConfig(U8* IP, S32* Port) {
    FILE* FP;
    char* TokPtr;
    U8 Conf[1024] = {0};
    U8* Tok = {0};
    U8 IPSet = FALSE;

    FP = NULL;
    FP = fopen("ingestion.conf", "rb");
    if (FP == NULL) {
        fprintf(stderr, "could not open config file");
        return 1;
    }
    
    if (fread(Conf, sizeof(U8), sizeof(Conf)/sizeof(U8), FP) > 0 && ferror(FP) != OK) {
        fprintf(stderr, "could not read config file");
        return 1;
    }

    fclose(FP);

    Tok = NULL;
    Tok = strtok_r(Conf, "\n", &TokPtr);

    while (Tok != NULL) {
        if (Tok[0] == '#' || strcmp(Tok, "") == 0) { //current line is a comment or empty
            Tok = strtok_r(NULL, "\n", &TokPtr);
            continue;
        } else if (IPSet == FALSE) { //this line is the IP address
            sprintf(IP, "%s", Tok);
            IPSet = TRUE;
        } else { //this line is the port number
            *Port = (S32)atoi(Tok);
            return OK;
        }
        Tok = strtok_r(NULL, "\n", &TokPtr);
    }

    fprintf(stderr, "could not parse config file");
    return 1;
}