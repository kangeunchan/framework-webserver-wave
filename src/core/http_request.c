#include "http_request.h"
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
    HttpRequest* request = malloc(sizeof(HttpRequest));
    if (!request) return NULL;
    memset(request, 0, sizeof(HttpRequest));

    // Parse the request line
    if (sscanf(raw_request, "%s %s %s", request->method, request->path, request->version) != 3) {
        free(request);
        return NULL;
    }

    // Parse headers and body (simplified version)
    const char* header_start = strchr(raw_request, '\n');
    if (!header_start) {
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
                http_request_destroy(request);
                return NULL;
            }
            request->header_count++;
            header_start += header_len + 1;
        }

        request->body = strdup(body_start + 4);  // Skip "\r\n\r\n"
        if (!request->body) {
            http_request_destroy(request);
            return NULL;
        }
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
        for (int i = 0; i < request->header_count; i++) {
            free(request->headers[i]);
        }
        free(request->body);
        free(request);
    }
}
