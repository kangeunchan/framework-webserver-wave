#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <pthread.h>

// Define colors for different log levels
#define COLOR_RESET   "\x1b[0m"
#define COLOR_INFO    "\x1b[32m" // Green
#define COLOR_WARN    "\x1b[33m" // Yellow
#define COLOR_ERROR   "\x1b[31m" // Red

static const char* LOG_LEVEL_NAMES[] = {
    "INFO",
    "WARN",
    "ERROR"
};

static const char* LOG_LEVEL_COLORS[] = {
    COLOR_INFO,
    COLOR_WARN,
    COLOR_ERROR
};

// Mutex for synchronizing log access
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
static FILE* log_file = NULL;

void log_init(const char* file_name) {
    log_file = fopen(file_name, "a");
    if (log_file == NULL) {
        fprintf(stderr, "Error opening log file: %s\n", file_name);
    }
}

void log_close() {
    if (log_file != NULL) {
        fclose(log_file);
    }
}

void log_message(LogLevel level, const char* format, ...) {
    va_list args;
    va_start(args, format);

    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

    // Lock the mutex before writing to the log
    pthread_mutex_lock(&log_mutex);

    // Print colored log message to stderr
    fprintf(stderr, "%s[%s] [%s] ", LOG_LEVEL_COLORS[level], time_str, LOG_LEVEL_NAMES[level]);
    vfprintf(stderr, format, args);
    fprintf(stderr, "%s\n", COLOR_RESET);

    // Print log message to file
    if (log_file != NULL) {
        fprintf(log_file, "[%s] [%s] ", time_str, LOG_LEVEL_NAMES[level]);
        vfprintf(log_file, format, args);
        fprintf(log_file, "\n");
        fflush(log_file);  // Ensure the log is written to the file immediately
    }

    // Unlock the mutex after writing to the log
    pthread_mutex_unlock(&log_mutex);

    va_end(args);
}
