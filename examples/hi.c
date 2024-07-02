#include "../include/wave.h"
#include "../src/utils/logger.h"
#include "../src/template/template.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 설정 모의 함수
const char* get_config(const char* key) {
    if (strcmp(key, "PORT") == 0) return "8080";
    return NULL;
}

// 라우트 핸들러 함수들
Response* home_handler(const Request* req) {
    logger("Home page requested", INFO);
    return create_response(200, "Welcome to Wave Framework!");
}

Response* about_handler(const Request* req) {
    logger("About page requested", INFO);
    char* content = render_template("about.html", "Wave Framework");
    Response* res = create_response(200, content);
    free(content);
    return res;
}

Response* api_handler(const Request* req) {
    logger("API endpoint requested", INFO);
    return create_response(200, "{\"message\": \"This is a JSON response\"}");
}

int main() {

    // 설정 로드
    const char* port_str = get_config("PORT");
    int port = port_str ? atoi(port_str) : 8080;

    // 라우트 등록
    routeAdd(GET, "/", home_handler);
    routeAdd(GET, "/about", about_handler);
    routeAdd(GET, "/api", api_handler);

    serverStart(port);

    return 0;
}