#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

void log_message(LogLevel level, const char* format, ...);
void log_init(const char* file_name);
void log_close();

#endif
