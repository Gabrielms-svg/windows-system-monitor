#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <windows.h>

__declspec(dllexport) void log_message(const char *format, ...) {
    FILE *logfile = fopen("log.txt", "a");
    if (!logfile) return;

    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    fprintf(logfile, "[%04d-%02d-%02d %02d:%02d:%02d] ",
            local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
            local->tm_hour, local->tm_min, local->tm_sec);

    va_list args;
    va_start(args, format);
    vfprintf(logfile, format, args);
    va_end(args);

    fprintf(logfile, "\n");
    fclose(logfile);
}
