#include "wave.h"
#include <stdio.h>

// 예제 JSON 응답 핸들러
void hello_handler(HttpRequest* request, HttpResponse* response) {
    JsonObject json_obj = {0};
    JSON_ADD_STRING(json_obj, "message", "Hello, World!");
    JSON_ADD_STRING(json_obj, "status", "success");
    set_json_response_from_object(response, 200, &json_obj);
}

// 사용자 정보를 반환하는 핸들러
void user_handler(HttpRequest* request, HttpResponse* response) {
    JsonObject json_obj = {0};
    JSON_ADD_STRING(json_obj, "name", "John Doe");
    JSON_ADD_STRING(json_obj, "email", "john.doe@example.com");
    JSON_ADD_STRING(json_obj, "role", "admin");
    set_json_response_from_object(response, 200, &json_obj);
}

// 404 Not Found 핸들러
void not_found_handler(HttpRequest* request, HttpResponse* response) {
    JsonObject json_obj = {0};
    JSON_ADD_STRING(json_obj, "error", "Not Found");
    JSON_ADD_STRING(json_obj, "message", "The requested resource was not found");
    set_json_response_from_object(response, 404, &json_obj);
}

// 미들웨어 예제: 로깅
void logging_middleware(HttpRequest* request, HttpResponse* response, int* next) {
    printf("Received %s request for %s\n", method_to_string(request->method), request->path);
    *next = 1; // 다음 미들웨어 또는 핸들러로 진행
}

int main() {
    WebFramework framework;
    init_framework(&framework);

    // 미들웨어 추가
    add_middleware(&framework, logging_middleware);

    // 라우트 추가
    add_route(&framework, GET, "/hello", hello_handler);
    add_route(&framework, GET, "/user", user_handler);

    // 404 Not Found 핸들러 추가
    add_route(&framework, GET, "/*", not_found_handler);

    printf("Starting server on port 8080...\n");
    start_server(&framework, 8080);

    return 0;
}