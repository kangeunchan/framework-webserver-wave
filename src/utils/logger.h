#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
    INFO,
    WARN,
    ERROR
} LEVEL;

void log(LEVEL level, const char* message);

#endif // LOGGER_H
