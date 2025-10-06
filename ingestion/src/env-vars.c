#include "ingestion.h"

//parse env vars file and set hmac key

U8 ReadEnvVars(V) {
    FILE* FP;
    char* TokPtr;
    U8 EnvFileContents[1024] = {0};
    U8* Tok = {0};

    FP = NULL;
    FP = fopen("ingestion.env", "rb");
    if (FP == NULL) {
        fprintf(stdout, "could not open config file\n");
        return NOK;
    }
    
    if (fread(EnvFileContents, sizeof(U8), sizeof(EnvFileContents)/sizeof(U8), FP) > 0 && ferror(FP) != OK) {
        fprintf(stdout, "could not read env file\n");
        return NOK;
    }

    fclose(FP);

    Tok = NULL;
    Tok = strtok_r(EnvFileContents, "\n", &TokPtr);

    while (Tok != NULL) {
        if (Tok[0] == '#' || strcmp(Tok, "") == 0) { //current line is a comment or empty
            Tok = strtok_r(NULL, "\n", &TokPtr);
            continue;
        } else { //this line is the hmac key
            if (setenv("HMACKEY", Tok, 1) != OK) {
                fprintf(stdout, "could not set env vars\n");
                return NOK;
            }

            return OK;
        }
        Tok = strtok_r(NULL, "\n", &TokPtr);
    }

    fprintf(stdout, "could not parse config file\n");
    return NOK;
}

U8 CleanUpEnvVars(V) {
    if (unsetenv("HMACKEY") != OK) {
        fprintf(stdout, "could not clean up env vars\n");
        return NOK;
    }
    return OK;
}