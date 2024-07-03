#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

void init_logger();
void log_message(LogLevel level, const char* format, ...);
void shutdown_logger();

#define log_info(...) log_message(LOG_INFO, __VA_ARGS__)
#define log_warning(...) log_message(LOG_WARNING, __VA_ARGS__)
#define log_error(...) log_message(LOG_ERROR, __VA_ARGS__)

#endif // LOGGER_H