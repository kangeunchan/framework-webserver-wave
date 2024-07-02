#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
    INFO,
    WARNING,
    ERROR
} LogType;

void logger(const char* message, LogType type);

#endif // LOGGER_H