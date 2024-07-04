#include "application.h"
#include <stdlib.h>

Application* application_create(void) {
    Application* app = malloc(sizeof(Application));
    if (!app) return NULL;

    app->container = container_create();
    app->http_server = http_server_create();
    app->router = router_create();

    if (!app->container || !app->http_server || !app->router) {
        application_destroy(app);
        return NULL;
    }

    return app;
}

void application_run(Application* app) {
    if (app && app->http_server && app->router) {
        http_server_start(app->http_server, app->router);
    }
}

void application_destroy(Application* app) {
    if (app) {
        container_destroy(app->container);
        http_server_destroy(app->http_server);
        router_destroy(app->router);
        free(app);
    }
}
