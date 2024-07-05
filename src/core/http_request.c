#include "http_request.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_HEADERS 20

struct HttpRequest {
    char method[10];
    char path[100];
    char version[10];
    char* headers[MAX_HEADERS];
    int header_count;
    char* body;
};

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
    log_message(LOG_INFO, "Parsed request line: method=%s, path=%s, version=%s", request->method, request->path, request->version);

    const char* header_start = strchr(raw_request, '\n');
    if (!header_start) {
        log_message(LOG_ERROR, "Failed to find header start in request: %s", raw_request);
        free(request);
        return NULL;
    }
    header_start += 1;
    const char* body_start = strstr(header_start, "\r\n\r\n");
    
    if (body_start) {
        const char* header_end = body_start;
        while (header_start < header_end && request->header_count < MAX_HEADERS) {
            int header_len = strchr(header_start, '\n') - header_start;
            request->headers[request->header_count] = strndup(header_start, header_len);
            if (!request->headers[request->header_count]) {
                log_message(LOG_ERROR, "Failed to allocate memory for header");
                http_request_destroy(request);
                return NULL;
            }
            log_message(LOG_INFO, "Added header: %s", request->headers[request->header_count]);
            request->header_count++;
            header_start += header_len + 1;
        }

        request->body = strdup(body_start + 4);
        if (!request->body) {
            log_message(LOG_ERROR, "Failed to allocate memory for body");
            http_request_destroy(request);
            return NULL;
        }
        log_message(LOG_INFO, "Parsed body: %s", request->body);
    } else {
        log_message(LOG_WARN, "No body found in request");
    }

    return request;
}

const char* http_request_get_method(HttpRequest* request) {
    return request->method;
}

const char* http_request_get_path(HttpRequest* request) {
    return request->path;
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
    }
}
