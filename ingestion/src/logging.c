#include "ingestion.h"

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