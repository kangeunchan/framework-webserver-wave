#include "logger.h"
#include <stdio.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void logger(const char* message, LogType type) {
    switch (type) {
        case INFO:
            printf(ANSI_COLOR_CYAN "[INFO] %s\n" ANSI_COLOR_RESET, message);
            break;
        case WARNING:
            printf(ANSI_COLOR_MAGENTA "[WARNING] %s\n" ANSI_COLOR_RESET, message);
            break;
        case ERROR:
            printf(ANSI_COLOR_BLUE "[ERROR] %s\n" ANSI_COLOR_RESET, message);
            break;
    }
}