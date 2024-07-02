#include "request.h"
#include <stdlib.h>
#include <string.h>

static HttpMethod parse_method(const char* method_str) {
    if (strcmp(method_str, "GET") == 0) return GET;
    if (strcmp(method_str, "POST") == 0) return POST;
    if (strcmp(method_str, "PUT") == 0) return PUT;
    if (strcmp(method_str, "DELETE") == 0) return DELETE;
    if (strcmp(method_str, "PATCH") == 0) return PATCH;
    if (strcmp(method_str, "HEAD") == 0) return HEAD;
    if (strcmp(method_str, "OPTIONS") == 0) return OPTIONS;
    return UNKNOWN;
}

Request* parse_request(const char* raw_request) {
    Request* request = malloc(sizeof(Request));
    char* request_copy = strdup(raw_request);
    
    char* method_str = strtok(request_copy, " ");
    request->method = parse_method(method_str);
    request->path = strdup(strtok(NULL, " "));
    request->body = strstr(raw_request, "\r\n\r\n");
    if (request->body) {
        request->body = strdup(request->body + 4); // Skip "\r\n\r\n"
    } else {
        request->body = strdup("");
    }
    
    free(request_copy);
    return request;
}

void free_request(Request* request) {
    if (request) {
        free(request->path);
        free(request->body);
        free(request);
    }
}