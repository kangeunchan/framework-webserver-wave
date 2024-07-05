#include "http_response.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_HEADERS 20

struct HttpResponse {
    int status_code;
    char* status_message;
    char* headers[MAX_HEADERS];
    int header_count;
    char* body;
};

HttpResponse* http_response_create() {
    log_message(LOG_INFO, "Creating HTTP response");
    HttpResponse* response = malloc(sizeof(HttpResponse));
    if (!response) {
        log_message(LOG_ERROR, "Failed to allocate memory for HTTP response");
        return NULL;
    }
    response->status_code = 200;
    response->status_message = strdup("OK");
    if (!response->status_message) {
        log_message(LOG_ERROR, "Failed to allocate memory for status message");
        free(response);
        return NULL;
    }
    response->header_count = 0;
    response->body = NULL;
    log_message(LOG_INFO, "HTTP response created successfully");
    return response;
}

void http_response_set_status(HttpResponse* response, int status_code, const char* status_message) {
    log_message(LOG_INFO, "Setting HTTP response status: %d %s", status_code, status_message);
    response->status_code = status_code;
    free(response->status_message);
    response->status_message = strdup(status_message);
    if (!response->status_message) {
        log_message(LOG_ERROR, "Failed to allocate memory for status message, using default");
        response->status_message = strdup("Unknown Status");
    }
}

void http_response_add_header(HttpResponse* response, const char* name, const char* value) {
    if (response->header_count < MAX_HEADERS) {
        log_message(LOG_INFO, "Adding header to HTTP response: %s: %s", name, value);
        char* header = malloc(strlen(name) + strlen(value) + 3);
        if (!header) {
            log_message(LOG_ERROR, "Failed to allocate memory for header");
            return;
        }
        sprintf(header, "%s: %s", name, value);
        response->headers[response->header_count++] = header;
    } else {
        log_message(LOG_WARN, "Max headers reached, cannot add header: %s: %s", name, value);
    }
}

void http_response_set_body(HttpResponse* response, const char* body) {
    log_message(LOG_INFO, "Setting HTTP response body");
    free(response->body);
    response->body = strdup(body);
    if (!response->body) {
        log_message(LOG_ERROR, "Failed to allocate memory for body, using empty body");
        response->body = strdup("");
    }
}

char* http_response_to_string(HttpResponse* response) {
    log_message(LOG_INFO, "Converting HTTP response to string");
    int total_length = strlen("HTTP/1.1 ") + 20 + strlen(response->status_message) + 2;
    for (int i = 0; i < response->header_count; i++) {
        total_length += strlen(response->headers[i]) + 2;
    }
    total_length += 2;
    if (response->body) {
        total_length += strlen(response->body);
    }

    char* response_str = malloc(total_length + 1);
    if (!response_str) {
        log_message(LOG_ERROR, "Failed to allocate memory for response string");
        return NULL;
    }

    char* current = response_str;

    current += sprintf(current, "HTTP/1.1 %d %s\r\n", response->status_code, response->status_message);

    for (int i = 0; i < response->header_count; i++) {
        current += sprintf(current, "%s\r\n", response->headers[i]);
    }

    current += sprintf(current, "\r\n%s", response->body ? response->body : "");

    log_message(LOG_INFO, "HTTP response converted to string successfully");
    return response_str;
}

void http_response_destroy(HttpResponse* response) {
    if (response) {
        log_message(LOG_INFO, "Destroying HTTP response");
        for (int i = 0; i < response->header_count; i++) {
            free(response->headers[i]);
        }
        free(response->status_message);
        free(response->body);
        free(response);
        log_message(LOG_INFO, "HTTP response destroyed successfully");
    }
}
