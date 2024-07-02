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
    char* fullResponse = malloc(2000);
    char* send_start_line;
    switch (response->statusCode) {
    case 200:
        send_start_line = "HTTP / 1.1 200 OK\r\n";
        break;
    case 404:
        send_start_line = "HTTP / 1.1 404 NOT FOUND\r\n";
        break;
    case 500:
        send_start_line = "HTTP / 1.1 500 REDIRECT\r\n";
        break;
    default:
        send_start_line = "HTTP / 1.1 200 OK\r\n";
    }
    snprintf(fullResponse, 2000,
        "%s%s\r\n%s",
        send_start_line, response->headers, response->body);
    return fullResponse;
}

void responseFree(WaveResponse *response) {
    free(response);
}