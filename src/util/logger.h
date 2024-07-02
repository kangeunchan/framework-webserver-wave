#ifndef WAVE_LOGGER_H
#define WAVE_LOGGER_H

void logInit(const char *logFile);
void logInfo(const char *message);
void logError(const char *message);
void logDebug(const char *message);

#endif // WAVE_LOGGER_H