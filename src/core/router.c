#include "router.h"
#include <stdlib.h>
#include <string.h>

#define MAX_ROUTES 100

typedef struct {
    char* method;
    char* path;
    void (*handler)(HttpRequest*, HttpResponse*);
} Route;

struct Router {
    Route routes[MAX_ROUTES];
    size_t count;
};

Router* router_create() {
    return calloc(1, sizeof(Router));
}

void router_add_route(Router* router, const char* method, const char* path, void (*handler)(HttpRequest*, HttpResponse*)) {
    if (router->count < MAX_ROUTES) {
        router->routes[router->count].method = strdup(method);
        router->routes[router->count].path = strdup(path);
        router->routes[router->count].handler = handler;
        router->count++;
    }
}

void router_handle_request(Router* router, HttpRequest* request, HttpResponse* response) {
    const char* method = http_request_get_method(request);
    const char* path = http_request_get_path(request);
    
    for (size_t i = 0; i < router->count; i++) {
        if (strcmp(router->routes[i].method, method) == 0 && strcmp(router->routes[i].path, path) == 0) {
            router->routes[i].handler(request, response);
            return;
        }
    }
    
    // 404 Not Found
    http_response_set_status(response, 404, "Not Found");
    http_response_set_body(response, "404 Not Found");
}

void router_destroy(Router* router) {
    for (size_t i = 0; i < router->count; i++) {
        free(router->routes[i].method);
        free(router->routes[i].path);
    }
    free(router);
}