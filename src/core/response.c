#include "response.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct WaveResponse {
    int statusCode;
    char headers[1000];
    char body[1000];
};

WaveResponse* responseCreate() {
    WaveResponse* response = (WaveResponse*)malloc(sizeof(WaveResponse));
    response->statusCode = 200;
    strcpy(response->headers, "Content-Type: text/html\r\n");
    strcpy(response->body, "");
    return response;
}

void responseSetStatus(WaveResponse *response, int statusCode) {
    response->statusCode = statusCode;
}

void responseSetHeader(WaveResponse *response, const char *key, const char *value) {
    char header[200];
    snprintf(header, sizeof(header), "%s: %s\r\n", key, value);
    strcat(response->headers, header);
}

void responseSetBody(WaveResponse *response, const char *body) {
    strncpy(response->body, body, sizeof(response->body) - 1);
}

char* responseToString(WaveResponse *response) {
    char* fullResponse = malloc(2000); // 충분한 크기 할당
    snprintf(fullResponse, 2000, 
            "HTTP/1.1 %d OK\r\n%s\r\n%s", 
            response->statusCode, response->headers, response->body);
    return fullResponse;
}

void responseFree(WaveResponse *response) {
    free(response);
}