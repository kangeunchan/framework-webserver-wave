#include "http_request.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_HEADERS 20

typedef struct HttpRequest {
    char method[10];
    char path[100];
    char version[10];
    char* headers[MAX_HEADERS];
    int header_count;
    char* body;
} HttpRequest;

static char* extract_line(const char** start, const char* end) {
    const char* eol = strchr(*start, '\n');
    if (!eol || eol > end) eol = end;
    size_t len = eol - *start;
    char* line = strndup(*start, len);
    *start = eol + 1;
    return line;
}

HttpRequest* http_request_parse(const char* raw_request) {
    log_message(LOG_INFO, "Parsing HTTP request");
    HttpRequest* request = malloc(sizeof(HttpRequest));
    if (!request) {
        log_message(LOG_ERROR, "Failed to allocate memory for HTTP request");
        return NULL;
    }
    memset(request, 0, sizeof(HttpRequest));

    if (sscanf(raw_request, "%s %s %s", request->method, request->path, request->version) != 3) {
        log_message(LOG_ERROR, "Failed to parse request line: %s", raw_request);
        free(request);
        return NULL;
    }

    const char* header_start = strchr(raw_request, '\n');
    if (!header_start) {
        log_message(LOG_ERROR, "Failed to find header start in request: %s", raw_request);
        free(request);
        return NULL;
    }
    header_start++;

    const char* body_start = strstr(header_start, "\r\n\r\n");
    const char* header_end = body_start ? body_start : header_start + strlen(header_start);

    while (header_start < header_end && request->header_count < MAX_HEADERS) {
        request->headers[request->header_count++] = extract_line(&header_start, header_end);
        if (!request->headers[request->header_count - 1]) {
            http_request_destroy(request);
            return NULL;
        }
    }

    if (body_start) {
        request->body = strdup(body_start + 4);
        if (!request->body) {
            log_message(LOG_ERROR, "Failed to allocate memory for body");
            http_request_destroy(request);
            return NULL;
        }
    }

    return request;
}

void http_request_destroy(HttpRequest* request) {
    if (request) {
        log_message(LOG_INFO, "Destroying HTTP request");
        for (int i = 0; i < request->header_count; i++) {
            free(request->headers[i]);
        }
        free(request->body);
        free(request);
        log_message(LOG_INFO, "HTTP request destroyed successfully");
    } else {
        log_message(LOG_WARN, "Tried to destroy NULL HTTP request");
    }
}

const char* http_request_get_method(HttpRequest* request) {
    return request ? request->method : NULL;
}

const char* http_request_get_path(HttpRequest* request) {
    return request ? request->path : NULL;
}

const char* http_request_get_version(HttpRequest* request) {
    return request ? request->version : NULL;
}

const char* http_request_get_body(HttpRequest* request) {
    return request ? request->body : NULL;
}
