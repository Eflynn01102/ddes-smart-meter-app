#include "ingestion.h"

V Log(S8 *Level, S8 *fmt, ...)
{
    // Get current time
    time_t CurrentTime = time(NULL);
    struct tm *TimeStruct = localtime(&CurrentTime);

    S8 timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", TimeStruct);

    fprintf(stdout, "[INGESTION] [%s] [%s] ", Level, timestamp);
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
    fflush(stdout);
}