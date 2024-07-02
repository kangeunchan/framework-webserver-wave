#include "logger.h"
#include <stdio.h>
#include <time.h>

static FILE *logFile = NULL;

void logInit(const char *logFilePath) {
    logFile = fopen(logFilePath, "a");
}

static void logMessage(const char *level, const char *message) {
    if (logFile) {
        time_t now = time(NULL);
        char timeString[26];
        ctime_r(&now, timeString);
        timeString[24] = '\0';  // 개행 문자 제거
        fprintf(logFile, "[%s] %s: %s\n", timeString, level, message);
        fflush(logFile);
    }
}

void logInfo(const char *message) {
    logMessage("INFO", message);
}

void logError(const char *message) {
    logMessage("ERROR", message);
}

void logDebug(const char *message) {
    logMessage("DEBUG", message);
}