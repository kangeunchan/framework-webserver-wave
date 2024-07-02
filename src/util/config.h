#ifndef WAVE_CONFIG_H
#define WAVE_CONFIG_H

void configLoad(const char *configFile);
char* configGet(const char *key);
int configGetInt(const char *key);

#endif // WAVE_CONFIG_H