#include "../include/wave.h"

#define INITIAL_ROUTES 10

Router* createRouter() {
    Router* router = (Router*)malloc(sizeof(Router));
    if (router == NULL) {
        log(ERROR, "ROUTER CREATE ERROR");
        return NULL;
    }

    router->routes = (Route*)malloc(INITIAL_ROUTES * sizeof(Route));
    if (router->routes == NULL) {
        free(router);
        log(ERROR, "ROUTER ROUTES ALLOCATION ERROR");
        return NULL;
    }

    router->route_count = 0;
    router->max_routes = INITIAL_ROUTES;
    return router;
}

void addRoute(Router* router, HttpMethod method, const char* path, RouteHandler handler) {
    if (router == NULL || path == NULL) {
        log(ERROR, "NULL POINTER ERROR IN ADD ROUTE");
        return;
    }

    if (router->route_count >= router->max_routes) {
        int new_max = router->max_routes * 2;
        Route* temp = (Route*)realloc(router->routes, new_max * sizeof(Route));
        if (temp == NULL) {
            log(ERROR, "ROUTER EXPANSION ERROR");
            return;
        }
        router->routes = temp;
        router->max_routes = new_max;
    }

    router->routes[router->route_count].method = method;
    router->routes[router->route_count].path = strdup(path);
    if (router->routes[router->route_count].path == NULL) {
        log(ERROR, "Memory allocation failed for route path");
        return;
    }
    router->routes[router->route_count].handler = handler;
    router->route_count++;
}

RouteHandler findRoute(Router* router, HttpMethod method, const char* path) {
    if (router == NULL || path == NULL) {
        return NULL;
    }

    for (int i = 0; i < router->route_count; i++) {
        if (router->routes[i].method == method && strcmp(router->routes[i].path, path) == 0) {
            return router->routes[i].handler;
        }
    }
    return NULL;
}

void destroyRouter(Router* router) {
    if (router) {
        for (int i = 0; i < router->route_count; i++) {
            free(router->routes[i].path);
        }
        free(router->routes);
        free(router);
    }
}
