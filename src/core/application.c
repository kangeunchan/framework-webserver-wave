#include "../include/wave.h"

Application* createApplication() {
    Application* app = (Application*)malloc(sizeof(Application));
    if (app == NULL) {
        log(ERROR, "APPLICATION CREATE ERROR");
        return NULL;
    }

    app->router = createRouter();
    if (app->router == NULL) {
        free(app);
        return NULL;
    }

    app->server_info.PORT = 8080;  // 기본 포트
    app->server_info.THREAD_POOL = threadPool(CREATE, 20, NULL, NULL, NULL);
    if (app->server_info.THREAD_POOL == NULL) {
        destroyRouter(app->router);
        free(app);
        return NULL;
    }

    return app;
}

void runApplication(Application* app) {
    if (app == NULL) {
        log(ERROR, "APPLICATION IS NULL");
        return;
    }

    server(app->server_info, START);
}

void destroyApplication(Application* app) {
    if (app == NULL) {
        return;
    }

    destroyRouter(app->router);
    threadPool(DESTORY, 0, app->server_info.THREAD_POOL, NULL, NULL);
    free(app);
}
