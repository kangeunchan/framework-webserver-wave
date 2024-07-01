#include "wave.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// GET 요청을 처리하는 핸들러
void home_handler(HttpRequest* request, HttpResponse* response) {
    strcpy(response->content, "Welcome! Send a POST request to /echo to test.");
    response->status_code = 200;
}

// POST 요청을 처리하는 핸들러
void echo_handler(HttpRequest* request, HttpResponse* response) {
    if (request->method != POST) {
        response->status_code = 405;
        strcpy(response->content, "Method Not Allowed");
        return;
    }

    // 요청 본문에서 직접 "message" 필드를 찾습니다.
    char* message_start = strstr(request->body, "\"message\"");
    char echoed_message[256] = "Echoed: ";
    
    if (message_start) {
        message_start = strchr(message_start, ':');
        if (message_start) {
            message_start++; // ':' 다음으로 이동
            while (*message_start == ' ' || *message_start == '"') message_start++; // 공백과 따옴표 건너뛰기
            
            char* message_end = strchr(message_start, '"');
            if (message_end) {
                size_t message_length = message_end - message_start;
                strncat(echoed_message, message_start, message_length);
            }
        }
    }

    // JSON 응답 생성
    JsonObject response_json = {0};
    JSON_ADD_STRING(response_json, "echo", echoed_message);
    set_json_response_from_object(response, 200, &response_json);
}

// 로깅 미들웨어
void logger_middleware(HttpRequest* request, HttpResponse* response, int* next) {
    printf("Received %s request for %s\n", method_to_string(request->method), request->path);
    if (request->method == POST) {
        printf("Request body: %s\n", request->body);
    }
    (*next)++;
}

int main() {
    WebFramework framework;
    init_framework(&framework);

    // 미들웨어 추가
    add_middleware(&framework, logger_middleware);

    // 라우트 추가
    add_route(&framework, GET, "/", home_handler);
    add_route(&framework, POST, "/echo", echo_handler);

    // 서버 시작
    printf("Server starting on port 8080...\n");
    start_server(&framework, 8080);

    return 0;
}