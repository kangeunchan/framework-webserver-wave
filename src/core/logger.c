#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define COLOR_RESET   "\x1b[0m"
#define COLOR_INFO    "\x1b[32m" // Green
#define COLOR_WARN    "\x1b[33m" // Yellow
#define COLOR_ERROR   "\x1b[31m" // Red
#define COLOR_DEBUG   "\x1b[34m" // Blue

#define MAX_LOG_QUEUE_SIZE 1000

static const char* LOG_LEVEL_NAMES[] = {
    "INFO",
    "WARN",
    "ERROR",
    "DEBUG"
};

static const char* LOG_LEVEL_COLORS[] = {
    COLOR_INFO,
    COLOR_WARN,
    COLOR_ERROR,
    COLOR_DEBUG
};

typedef struct {
    LogLevel level;
    char* message;
} LogEntry;

static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t log_cond = PTHREAD_COND_INITIALIZER;
static FILE* log_file = NULL;
static LogEntry* log_queue = NULL;
static int log_queue_size = 0;
static int log_queue_capacity = 0;
static int shutdown_logger = 0;
static pthread_t logger_thread;

static void* logger_worker(void* arg) {
    while (1) {
        pthread_mutex_lock(&log_mutex);
        while (log_queue_size == 0 && !shutdown_logger) {
            pthread_cond_wait(&log_cond, &log_mutex);
        }

        if (shutdown_logger && log_queue_size == 0) {
            pthread_mutex_unlock(&log_mutex);
            break;
        }

        LogEntry entry = log_queue[0];
        memmove(&log_queue[0], &log_queue[1], (log_queue_size - 1) * sizeof(LogEntry));
        log_queue_size--;

        pthread_mutex_unlock(&log_mutex);

        time_t now = time(NULL);
        struct tm t;
        localtime_r(&now, &t);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &t);

        fprintf(stderr, "%s[%s] [%s] %s%s\n", LOG_LEVEL_COLORS[entry.level], time_str, LOG_LEVEL_NAMES[entry.level], entry.message, COLOR_RESET);

        if (log_file != NULL) {
            fprintf(log_file, "[%s] [%s] %s\n", time_str, LOG_LEVEL_NAMES[entry.level], entry.message);
            fflush(log_file);
        }

        free(entry.message);
    }

    return NULL;
}

void log_init(const char* file_name) {
    log_file = fopen(file_name, "a");
    if (log_file == NULL) {
        fprintf(stderr, "Error opening log file: %s\n", file_name);
        return;
    }

    log_queue_capacity = MAX_LOG_QUEUE_SIZE;
    log_queue = malloc(sizeof(LogEntry) * log_queue_capacity);
    if (log_queue == NULL) {
        fprintf(stderr, "Error allocating memory for log queue\n");
        fclose(log_file);
        return;
    }

    pthread_create(&logger_thread, NULL, logger_worker, NULL);
}

void log_close() {
    pthread_mutex_lock(&log_mutex);
    shutdown_logger = 1;
    pthread_cond_signal(&log_cond);
    pthread_mutex_unlock(&log_mutex);

    pthread_join(logger_thread, NULL);

    if (log_file != NULL) {
        fclose(log_file);
    }
    free(log_queue);
    pthread_mutex_destroy(&log_mutex);
    pthread_cond_destroy(&log_cond);
}

void log_message(LogLevel level, const char* format, ...) {
    va_list args;
    va_start(args, format);

    char* message;
    vasprintf(&message, format, args);

    pthread_mutex_lock(&log_mutex);

    if (log_queue_size < log_queue_capacity) {
        log_queue[log_queue_size].level = level;
        log_queue[log_queue_size].message = message;
        log_queue_size++;
        pthread_cond_signal(&log_cond);
    } else {
        fprintf(stderr, "Log queue full, discarding message: %s\n", message);
        free(message);
    }

    pthread_mutex_unlock(&log_mutex);

    va_end(args);
}