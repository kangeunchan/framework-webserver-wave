#include "../include/wave.h"

Request* parseRequest(char* requestStr) {
    if (requestStr == NULL) {
        log(ERROR, "NULL REQUEST STRING");
        return NULL;
    }

    Request* request = (Request*)malloc(sizeof(Request));
    if (request == NULL) {
        log(ERROR, "Memory allocation failed for Request");
        return NULL;
    }

    char* methodStr = strtok(requestStr, " ");
    if (methodStr == NULL) {
        log(ERROR, "Invalid request string format");
        free(request);
        return NULL;
    }

    if (strcmp(methodStr, "GET") == 0) request->method = GET;
    else if (strcmp(methodStr, "POST") == 0) request->method = POST;
    else if (strcmp(methodStr, "PUT") == 0) request->method = PUT;
    else if (strcmp(methodStr, "DELETE") == 0) request->method = DELETE;
    else if (strcmp(methodStr, "PATCH") == 0) request->method = PATCH;
    else if (strcmp(methodStr, "HEAD") == 0) request->method = HEAD;
    else if (strcmp(methodStr, "OPTIONS") == 0) request->method = OPTIONS;
    else request->method = UNKNOWN;

    char* path = strtok(NULL, " ");
    if (path == NULL) {
        log(ERROR, "Invalid request path format");
        free(request);
        return NULL;
    }
    request->path = strdup(path);
    if (request->path == NULL) {
        log(ERROR, "Memory allocation failed for request path");
        free(request);
        return NULL;
    }

    // Simple body parsing (this should be improved for real-world use)
    char* body = strstr(requestStr, "\r\n\r\n");
    if (body) {
        body += 4;  // Move past "\r\n\r\n"
        request->body = strdup(body);
        if (request->body == NULL) {
            log(ERROR, "Memory allocation failed for request body");
            free(request->path);
            free(request);
            return NULL;
        }
    } else {
        request->body = NULL;
    }

    return request;
}

void freeRequest(Request* request) {
    if (request != NULL) {
        free(request->path);
        free(request->body);
        free(request);
    }
}
