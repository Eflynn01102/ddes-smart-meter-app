#include "ingestion.h"

//fire and forget approach
V Alert(S8* Msg, ConfigType Conf) {
    CURL *Curl = curl_easy_init();

    if (Curl == FALSE) {
        return;
    }

    S8 MessagePayload[1024] = {0};

    sprintf(MessagePayload, "{\"clientID\":\"client-131\",\"title\":\"Ingestion Service Message\",\"severity\":\"info\",\"message\":\"%s\"}", Msg);
    
    curl_easy_setopt(Curl, CURLOPT_TIMEOUT_MS, 500L); //not hanging around here
    curl_easy_setopt(Curl, CURLOPT_URL, Conf.AlertsHost);
    curl_easy_setopt(Curl, CURLOPT_PORT, Conf.AlertsPort);
    curl_easy_setopt(Curl, CURLOPT_POSTFIELDSIZE, -1L);
    curl_easy_setopt(Curl, CURLOPT_POSTFIELDS, MessagePayload);
    curl_easy_perform(Curl);
    curl_easy_cleanup(Curl);
}

V Log(S8 *Level, S8 *fmt, ...)
{
    time_t CurrentTime = time(NULL);
    struct tm *TimeStruct = localtime(&CurrentTime);

    S8 Timestamp[20];
    strftime(Timestamp, sizeof(Timestamp), "%Y-%m-%d %H:%M:%S", TimeStruct);

    fprintf(stdout, "[INGESTION] [%s] [%s] ", Timestamp, Level);

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
    fflush(stdout);
}