#include "wave.h"
#include <ctype.h>

static void trim(char* str) {
    if (!str) return;
    
    char* start = str;
    char* end = str + strlen(str) - 1;

    while (isspace((unsigned char)*start)) start++;
    while (end > start && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';
    memmove(str, start, end - start + 2);
}

static void parse_json_pair(const char* key, const char* value, HttpRequest* request) {
    if (!key || !value || !request) return;

    if (strcmp(key, "path") == 0) {
        strncpy(request->path, value, sizeof(request->path) - 1);
        request->path[sizeof(request->path) - 1] = '\0';
    } else if (strcmp(key, "method") == 0) {
        if (strcmp(value, "GET") == 0) request->method = GET;
        else if (strcmp(value, "POST") == 0) request->method = POST;
        else if (strcmp(value, "PUT") == 0) request->method = PUT;
        else if (strcmp(value, "DELETE") == 0) request->method = DELETE;
        else if (strcmp(value, "PATCH") == 0) request->method = PATCH;
    }
}

void json_parser(const char* request_str, HttpRequest* request) {
    if (!request_str || !request) return;

    const char* body_start = strstr(request_str, "\r\n\r\n");
    if (body_start) {
        body_start += 4;
        
        char body_copy[MAX_REQUEST_SIZE];
        strncpy(body_copy, body_start, sizeof(body_copy) - 1);
        body_copy[sizeof(body_copy) - 1] = '\0';

        char* token = strtok(body_copy, ",");
        while (token) {
            char* colon = strchr(token, ':');
            if (colon) {
                *colon = '\0';
                char* key = token;
                char* value = colon + 1;
                
                trim(key);
                trim(value);

                size_t value_len = strlen(value);
                if (value_len >= 2 && value[0] == '"' && value[value_len - 1] == '"') {
                    value[value_len - 1] = '\0';
                    value++;
                }

                parse_json_pair(key, value, request);
            }
            token = strtok(NULL, ",");
        }
    }

    strncpy(request->body, body_start ? body_start : "", sizeof(request->body) - 1);
    request->body[sizeof(request->body) - 1] = '\0';
}