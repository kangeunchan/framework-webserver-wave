#include "wave.h"
#include <ctype.h>

void trim(char* str) {
    char* start = str;
    char* end = str + strlen(str) - 1;

    while(isspace((unsigned char)*start)) start++;
    while(end > start && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';
    memmove(str, start, end - start + 2);
}

void parse_json_pair(char* key, char* value, HttpRequest* request) {
    trim(key);
    trim(value);

    // Remove quotes from the value
    if (value[0] == '"' && value[strlen(value) - 1] == '"') {
        value[strlen(value) - 1] = '\0';
        value++;
    }

    // You can add more fields here as needed
    if (strcmp(key, "path") == 0) {
        strncpy(request->path, value, sizeof(request->path) - 1);
    } else if (strcmp(key, "method") == 0) {
        if (strcmp(value, "GET") == 0) request->method = GET;
        else if (strcmp(value, "POST") == 0) request->method = POST;
        else if (strcmp(value, "PUT") == 0) request->method = PUT;
        else if (strcmp(value, "DELETE") == 0) request->method = DELETE;
        else if (strcmp(value, "PATCH") == 0) request->method = PATCH;
    }
    // Add more fields as needed
}

void Json_parser(char* request_str, HttpRequest* request) {
    char* body_start = strstr(request_str, "\r\n\r\n");
    if (body_start) {
        body_start += 4;  // Skip \r\n\r\n
        
        char* token = strtok(body_start, ",");
        while (token != NULL) {
            char* colon = strchr(token, ':');
            if (colon) {
                *colon = '\0';
                char* key = token;
                char* value = colon + 1;
                parse_json_pair(key, value, request);
            }
            token = strtok(NULL, ",");
        }
    }

    // Copy the entire body to request->body
    strncpy(request->body, body_start, sizeof(request->body) - 1);
    request->body[sizeof(request->body) - 1] = '\0';
}