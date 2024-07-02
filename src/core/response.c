#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Response* create_response(int status, const char* body) {
    Response* response = malloc(sizeof(Response));
    response->status = status;
    response->body = strdup(body);
    return response;
}

char* serialize_response(const Response* response) {
    char* status_text;
    switch (response->status) {
        case 200: status_text = "OK"; break;
        case 404: status_text = "Not Found"; break;
        default: status_text = "Internal Server Error";
    }
    
    char* response_str = malloc(1024); // Adjust size as needed
    snprintf(response_str, 1024, "HTTP/1.1 %d %s\r\nContent-Type: text/plain\r\nContent-Length: %zu\r\n\r\n%s",
            response->status, status_text, strlen(response->body), response->body);
    
    return response_str;
}

void free_response(Response* response) {
    if (response) {
        free(response->body);
        free(response);
    }
}