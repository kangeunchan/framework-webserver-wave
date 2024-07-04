#include "../include/wave.h"

Response* createResponse() {
    Response* response = (Response*)malloc(sizeof(Response));
    if (response == NULL) {
        log(ERROR, "RESPONSE CREATE ERROR");
        return NULL;
    }

    response->status_code = 200;
    response->headers = NULL;
    response->body = NULL;

    return response;
}

void setStatusCode(Response* response, int status_code) {
    if (response) {
        response->status_code = status_code;
    }
}

void addHeader(Response* response, const char* key, const char* value) {
    if (response && key && value) {
        char* new_header = malloc(strlen(key) + strlen(value) + 3); // key: value\0
        if (new_header == NULL) {
            log(ERROR, "HEADER MEMORY ALLOCATION FAILED");
            return;
        }
        sprintf(new_header, "%s: %s", key, value);
        
        if (response->headers) {
            char* temp = realloc(response->headers, strlen(response->headers) + strlen(new_header) + 2); // \n\0
            if (temp == NULL) {
                log(ERROR, "HEADER REALLOCATION FAILED");
                free(new_header);
                return;
            }
            response->headers = temp;
            strcat(response->headers, "\n");
            strcat(response->headers, new_header);
            free(new_header);
        } else {
            response->headers = new_header;
        }
    }
}

void setBody(Response* response, const char* body) {
    if (response && body) {
        free(response->body);  // Free existing body if any
        response->body = strdup(body);
        if (response->body == NULL) {
            log(ERROR, "BODY MEMORY ALLOCATION FAILED");
        }
    }
}

void destroyResponse(Response* response) {
    if (response) {
        free(response->headers);
        free(response->body);
        free(response);
    }
}
