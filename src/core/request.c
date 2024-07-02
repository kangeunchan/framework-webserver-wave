#include "wave.h"
#include "request.h"
#include <stdlib.h>
#include <string.h>
struct WaveRequest {
    char method[10];
    char path[100];
    char headers[1000];
    char body[1000];
};

WaveRequest* requestCreate() {
    return (WaveRequest*)calloc(1, sizeof(WaveRequest));
}

void requestParse(WaveRequest *request, const char *rawRequest) {
    sscanf(rawRequest, "%s %s", request->method, request->path);
}

char* requestGetHeader(WaveRequest *request, const char *key) {
    return NULL;
}

char* requestGetParam(WaveRequest *request, const char *key) {
    return NULL;
}

void requestFree(WaveRequest *request) {
    free(request);
}

const char* requestGetMethod(WaveRequest *request) {
    return request->method;
}

const char* requestGetPath(WaveRequest *request) {
    return request->path;
}