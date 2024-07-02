#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* key;
    char* value;
} Config;

static Config* configs = NULL;
static int config_count = 0;

void load_config(const char* file) {
    FILE* fp = fopen(file, "r");
    if (!fp) return;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char* key = strtok(line, "=");
        char* value = strtok(NULL, "\n");
        configs = realloc(configs, sizeof(Config) * (config_count + 1));
        configs[config_count].key = strdup(key);
        configs[config_count].value = strdup(value);
        config_count++;
    }
    fclose(fp);
}

const char* get_config_value(const char* key) {
    for (int i = 0; i < config_count; i++) {
        if (strcmp(configs[i].key, key) == 0) {
            return configs[i].value;
        }
    }
    return NULL;
}
