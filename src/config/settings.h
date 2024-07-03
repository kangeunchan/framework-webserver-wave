#ifndef SETTING_H
#define SETTING_H

typedef struct {
    char *key;
    char *value;
} KeyValue;

void waveFileReader(char *filename, char *path);

#endif // SETTING_H