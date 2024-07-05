#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_DEBUG
} LogLevel;

void log_init(const char* file_name);
void log_close();
void log_message(LogLevel level, const char* format, ...);

#endif // LOGGER_H
