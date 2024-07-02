#include "../include/wave.h"
#include "../src/utils/logger.h"
#include "../src/template/template.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 데이터베이스 모의 함수
void db_connect() {
    printf("Connected to database\n");
}

// 설정 모의 함수
const char* get_config(const char* key) {
    if (strcmp(key, "PORT") == 0) return "8080";
    return NULL;
}

// 미들웨어 모의 함수
void auth_middleware(const Request* req, Response* res) {
    printf("Auth middleware: Checking authorization\n");
    // 실제로는 여기서 인증 로직을 구현합니다.
}

// 라우트 핸들러 함수들
Response* home_handler(const Request* req) {
    log_info("Home page requested");
    return create_response(200, "Welcome to Wave Framework!");
}

Response* about_handler(const Request* req) {
    log_info("About page requested");
    char* content = render_template("about.html", "Wave Framework");
    Response* res = create_response(200, content);
    free(content);
    return res;
}

Response* api_handler(const Request* req) {
    log_info("API endpoint requested");
    return create_response(200, "{\"message\": \"This is a JSON response\"}");
}

int main() {
    // 데이터베이스 연결
    db_connect();

    // 설정 로드
    const char* port_str = get_config("PORT");
    int port = port_str ? atoi(port_str) : 8080;

    // 라우트 등록
    add_route(GET, "/", home_handler);
    add_route(GET, "/about", about_handler);
    add_route(GET, "/api", api_handler);

    // 서버 시작
    printf("Starting server on port %d\n", port);
    start_server(port);

    return 0;
}