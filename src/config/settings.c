#include "../include/wave.h"

#define INITIAL_CAPACITY 10

void waveFileReader(char *filename, char *path) {
    FILE *fp;
    char pathMessage[PATH_MAX];
    char fullPath[PATH_MAX];
    char *extension;
    KeyValue *keyValuePairs = NULL;
    int capacity = INITIAL_CAPACITY;
    int size = 0;

    extension = strrchr(filename, '.');
    if (extension == NULL || strcmp(extension, ".wave") != 0) {
        log(ERROR, "FILE EXTENSION IS NOT .wave");
        return;
    }

    if (filename == NULL || path == NULL) {
        log(ERROR, "FILE NAME OR PATH IS NULL");
        return;
    }

    snprintf(pathMessage, PATH_MAX, "SETTING FILE PATH: %s/%s", path, filename);
    snprintf(fullPath, PATH_MAX, "%s/%s", path, filename);

    log(INFO, pathMessage);

    fp = fopen(fullPath, "r");
    if (fp == NULL) {
        log(ERROR, "FILE OPEN ERROR");
        return;
    }

    keyValuePairs = (KeyValue *)malloc(capacity * sizeof(KeyValue));
    if (keyValuePairs == NULL) {
        log(ERROR, "MEMORY ALLOCATION ERROR");
        fclose(fp);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "=");

        if (key != NULL && value != NULL) {
            char *trimmedKey = strdup(key);
            char *trimmedValue = strdup(value);

            if (trimmedKey != NULL && trimmedValue != NULL) {
                keyValuePairs[size].key = trimmedKey;
                keyValuePairs[size].value = trimmedValue;
                size++;

                if (size >= capacity) {
                    capacity *= 2;
                    KeyValue *temp = (KeyValue *)realloc(keyValuePairs, capacity * sizeof(KeyValue));
                    if (temp == NULL) {
                        log(ERROR, "MEMORY REALLOCATION ERROR");
                        break;
                    }
                    keyValuePairs = temp;
                }
            } else {
                free(trimmedKey);
                free(trimmedValue);
                log(ERROR, "MEMORY ALLOCATION ERROR FOR KEY OR VALUE");
            }
        }
    }

    fclose(fp);

    for (int i = 0; i < size; ++i) {
        char logMessage[1024];
        snprintf(logMessage, sizeof(logMessage), "FILE: %-5s || KEY: %-5s || VALUE: %s", filename, keyValuePairs[i].key, keyValuePairs[i].value);
        log(INFO, logMessage);
    }

    for (int i = 0; i < size; ++i) {
        free(keyValuePairs[i].key);
        free(keyValuePairs[i].value);
    }
    free(keyValuePairs);
}