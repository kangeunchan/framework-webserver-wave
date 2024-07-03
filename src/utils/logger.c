#include "../include/wave.h"

#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RED "\x1b[31m"
#define COLOR_GRAY "\x1b[37m"

void log(LEVEL level, const char* message) {
    switch (level) {
        case INFO:
            printf(COLOR_GREEN "[INFO] %s\n" COLOR_GRAY, message);
            break;
        case WARN:
            printf(COLOR_YELLOW "[WARN] %s\n" COLOR_GRAY, message);
            break;
        case ERROR:
            printf(COLOR_RED "[ERROR] %s\n" COLOR_GRAY, message);
            exit(1);
            break;
        default:
            printf(COLOR_GRAY "[UNKNOWN] %s\n" COLOR_GRAY, message);
            break;
    }
}