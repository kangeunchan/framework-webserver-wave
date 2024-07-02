#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONFIG_ITEMS 100
#define MAX_KEY_LENGTH 50
#define MAX_VALUE_LENGTH 100

static struct {
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
} configItems[MAX_CONFIG_ITEMS];

static int configItemCount = 0;

void configLoad(const char *configFile) {
    FILE *file = fopen(configFile, "r");
    if (!file) {
        perror("Error opening config file");
        return;
    }

    char line[MAX_KEY_LENGTH + MAX_VALUE_LENGTH + 2];
    while (fgets(line, sizeof(line), file) && configItemCount < MAX_CONFIG_ITEMS) {
        char *equalSign = strchr(line, '=');
        if (equalSign) {
            *equalSign = '\0';
            strncpy(configItems[configItemCount].key, line, MAX_KEY_LENGTH - 1);
            strncpy(configItems[configItemCount].value, equalSign + 1, MAX_VALUE_LENGTH - 1);
            configItems[configItemCount].value[strcspn(configItems[configItemCount].value, "\n")] = 0;
            configItemCount++;
        }
    }

    fclose(file);
}

char* configGet(const char *key) {
    for (int i = 0; i < configItemCount; i++) {
        if (strcmp(configItems[i].key, key) == 0) {
            return configItems[i].value;
        }
    }
    return NULL;
}

int configGetInt(const char *key) {
    char *value = configGet(key);
    return value ? atoi(value) : 0;
}