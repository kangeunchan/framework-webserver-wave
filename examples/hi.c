#include "../include/wave.h"
#include "../src/utils/logger.h"
#include "../src/utils/config.h"
#include "../src/template/template.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>  // sleep() 함수를 위해

#define DEFAULT_PORT 8080

static volatile sig_atomic_t keep_running = 1;


// 라우트 핸들러 함수들
Response* home_handler(const Request* req) {
    log_info("Home page requested");
    return create_response(200, "Welcome to Wave Framework!");
}

Response* about_handler(const Request* req) {
    log_info("About page requested");
    char* content = render_template("about.html", "Wave Framework");
    if (!content) {
        log_error("Failed to render about.html template");
        return create_response(500, "Internal Server Error");
    }
    Response* res = create_response(200, content);
    free(content);
    return res;
}

Response* api_handler(const Request* req) {
    log_info("API endpoint requested");
    return create_response(200, "{\"message\": \"This is a JSON response\"}");
}

int main() {
    signal(SIGINT, signal_handler);

    // 로거 초기화
    init_logger();

    // 설정 로드
    load_config("config.ini");
    const char* port_str = get_config_value("PORT");
    int port = port_str ? atoi(port_str) : DEFAULT_PORT;

    if (port <= 0 || port > 65535) {
        log_error("Invalid port number: %d", port);
        shutdown_logger();
        return EXIT_FAILURE;
    }
if (routeAdd(GET, "/", home_handler) != 0 ||
    routeAdd(GET, "/about", about_handler) != 0 ||
    routeAdd(GET, "/api", api_handler) != 0) {
    log_error("Failed to add routes");
    shutdown_logger();
    return EXIT_FAILURE;
}
    log_info("Starting server on port %d", port);
    serverStart(port);

    while (keep_running) {
        // 메인 루프
        // 여기에 추가적인 관리 작업을 수행할 수 있습니다.
        sleep(1);
    }

    log_info("Server shutting down...");
    shutdown_logger();

    return EXIT_SUCCESS;
}