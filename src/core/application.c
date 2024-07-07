#include "application.h"
#include <stdlib.h>

// Application 객체 생성 및 초기화
Application* application_create(void) {
    Application* app = malloc(sizeof(Application));
    if (!app) return NULL;

    // 각 구성 요소 생성
    if (!(app->container = container_create()) ||
        !(app->http_server = http_server_create()) ||
        !(app->router = router_create())) {
        application_destroy(app);  // 생성 실패 시 자원 정리
        return NULL;
    }

    return app;
}

// Application 객체 실행
void application_run(Application* app) {
    if (app && app->http_server && app->router) {
        http_server_start(app->http_server, app->router);
    }
}

// Application 객체 파괴
void application_destroy(Application* app) {
    if (app) {
        container_destroy(app->container);
        http_server_destroy(app->http_server);
        router_destroy(app->router);
        free(app);
    }
}
