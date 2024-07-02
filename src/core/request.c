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
    // 간단한 파싱 구현. 실제로는 더 복잡할 수 있습니다.
    sscanf(rawRequest, "%s %s", request->method, request->path);
    // 헤더와 바디 파싱은 여기서 생략
}

char* requestGetHeader(WaveRequest *request, const char *key) {
    // 간단한 구현. 실제로는 헤더를 파싱하고 검색해야 합니다.
    return NULL;
}

char* requestGetParam(WaveRequest *request, const char *key) {
    // URL 파라미터 파싱 및 검색 로직 구현 필요
    return NULL;
}

void requestFree(WaveRequest *request) {
    free(request);
}

// 추가 함수
const char* requestGetMethod(WaveRequest *request) {
    return request->method;
}

const char* requestGetPath(WaveRequest *request) {
    return request->path;
}