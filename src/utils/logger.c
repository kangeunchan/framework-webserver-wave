#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

#define MAX_LOG_LENGTH 1024
#define LOG_QUEUE_SIZE 1000

typedef struct {
    LogLevel level;
    char message[MAX_LOG_LENGTH];
} LogEntry;

static LogEntry log_queue[LOG_QUEUE_SIZE];
static int queue_front = 0;
static int queue_rear = 0;
static int queue_size = 0;
static pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;
static pthread_t logger_thread;
static volatile int keep_logging = 1;

static const char* level_strings[] = {
    "INFO",
    "WARNING",
    "ERROR"
};

static const char* level_colors[] = {
    "\x1b[32m", // Green for INFO
    "\x1b[33m", // Yellow for WARNING
    "\x1b[31m"  // Red for ERROR
};

static void* logger_worker(void* arg) {
    while (keep_logging) {
        pthread_mutex_lock(&queue_mutex);
        while (queue_size == 0 && keep_logging) {
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }

        if (!keep_logging) {
            pthread_mutex_unlock(&queue_mutex);
            break;
        }

        LogEntry entry = log_queue[queue_front];
        queue_front = (queue_front + 1) % LOG_QUEUE_SIZE;
        queue_size--;

        pthread_mutex_unlock(&queue_mutex);

        time_t now;
        time(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

        printf("%s %s[%s]\x1b[0m %s\n", timestamp, level_colors[entry.level], level_strings[entry.level], entry.message);
        fflush(stdout);
    }

    return NULL;
}

void init_logger() {
    pthread_create(&logger_thread, NULL, logger_worker, NULL);
}

void log_message(LogLevel level, const char* format, ...) {
    va_list args;
    va_start(args, format);

    LogEntry entry;
    entry.level = level;
    vsnprintf(entry.message, MAX_LOG_LENGTH, format, args);

    va_end(args);

    pthread_mutex_lock(&queue_mutex);

    if (queue_size == LOG_QUEUE_SIZE) {
        // Queue is full, discard the oldest log
        queue_front = (queue_front + 1) % LOG_QUEUE_SIZE;
        queue_size--;
    }

    log_queue[queue_rear] = entry;
    queue_rear = (queue_rear + 1) % LOG_QUEUE_SIZE;
    queue_size++;

    pthread_cond_signal(&queue_cond);
    pthread_mutex_unlock(&queue_mutex);
}

void shutdown_logger() {
    pthread_mutex_lock(&queue_mutex);
    keep_logging = 0;
    pthread_cond_signal(&queue_cond);
    pthread_mutex_unlock(&queue_mutex);

    pthread_join(logger_thread, NULL);
}