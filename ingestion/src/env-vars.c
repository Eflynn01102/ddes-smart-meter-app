#include "ingestion.h"

//parse env vars file and set hmac key

U8 ReadEnvVars(V) {
    FILE* FP;
    char* TokPtr;
    U8 EnvFileContents[1024] = {0};
    U8* Tok = {0};
    S32 LinesRead = 0;

    FP = NULL;
    FP = fopen("ingestion.env", "rb");
    if (FP == NULL) {
        LogErr("could not open config file\n");
        return NOK;
    }
    
    if (fread(EnvFileContents, sizeof(U8), sizeof(EnvFileContents)/sizeof(U8), FP) > 0 && ferror(FP) != OK) {
        LogErr("could not read env file\n");
        return NOK;
    }

    fclose(FP);

    Tok = NULL;
    Tok = strtok_r(EnvFileContents, "\n", &TokPtr);

    while (Tok != NULL) {
        if (Tok[0] == '#' || strcmp(Tok, "") == 0) { //current line is a comment or empty
            Tok = strtok_r(NULL, "\n", &TokPtr);
            continue;
        } else if (LinesRead == 0) { //this line is the hmac key
            if (setenv("HMACKEY", Tok, 1) != OK) {
                LogErr("could not set env var HMACKEY\n");
                return NOK;
            }
            LinesRead++;
        } else {
            if (setenv("CLIENTFW", Tok, 1) != OK) {
                LogErr("could not set env var CLIENTFW\n");
                return NOK;
            }
            return OK;
        }
        Tok = strtok_r(NULL, "\n", &TokPtr);
    }

    LogErr("could not parse config file\n");
    return NOK;
}

U8 CleanUpEnvVars(V) {
    if (unsetenv("HMACKEY") != OK) {
        LogErr("could not clean up env vars\n");
        return NOK;
    }
    return OK;
}