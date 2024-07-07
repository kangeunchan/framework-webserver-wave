#include "json.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PAIRS 100

static char* skip_whitespace(const char* str) {
    while (isspace(*str)) str++;
    return (char*)str;
}

static char* parse_string(const char* str, char** end) {
    if (*str != '"') return NULL;
    str++; // Skip opening quote
    const char* start = str;
    while (*str && *str != '"') {
        if (*str == '\\') str++; // Skip escaped characters
        str++;
    }
    if (!*str) return NULL; // Unterminated string

    int len = str - start;
    char* result = malloc(len + 1);
    if (!result) return NULL;
    strncpy(result, start, len);
    result[len] = '\0';

    *end = (char*)str + 1; // Skip closing quote
    return result;
}

static int add_pair(Json* json, char* key, char* value) {
    if (json->pair_count >= MAX_PAIRS) {
        free(key);
        free(value);
        return 0;
    }
    json->pairs[json->pair_count].key = key;
    json->pairs[json->pair_count].value = value;
    json->pair_count++;
    return 1;
}

Json* json_parse(const char* raw_json) {
    Json* json = malloc(sizeof(Json));
    if (!json) return NULL;

    json->pair_count = 0;
    const char* p = skip_whitespace(raw_json);
    if (*p != '{') {
        free(json);
        return NULL;
    }
    p++;

    while (*p && *p != '}') {
        p = skip_whitespace(p);
        char* key = parse_string(p, (char**)&p);
        if (!key) break;

        p = skip_whitespace(p);
        if (*p != ':') {
            free(key);
            break;
        }
        p++;

        p = skip_whitespace(p);
        char* value = parse_string(p, (char**)&p);
        if (!value) {
            free(key);
            break;
        }

        if (!add_pair(json, key, value)) {
            free(key);
            free(value);
            break;
        }

        p = skip_whitespace(p);
        if (*p == ',') p++;
    }

    if (*p != '}') {
        for (int i = 0; i < json->pair_count; i++) {
            free(json->pairs[i].key);
            free(json->pairs[i].value);
        }
        free(json);
        return NULL;
    }

    return json;
}
