#ifndef JSON_H
#define JSON_H

typedef struct {
    char* key;
    char* value;
} JsonPair;

typedef struct {
    JsonPair* pairs;
    int pair_count;
} Json;

Json* json_parse(const char* raw_json);

#endif