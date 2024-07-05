#include "router.h"
#include "logger.h"
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
    log_message(LOG_INFO, "Creating router");
    Router* router = calloc(1, sizeof(Router));
    if (router == NULL) {
        log_message(LOG_ERROR, "Failed to allocate memory for router");
    } else {
        log_message(LOG_INFO, "Router created successfully");
    }
    return router;
}

void router_add_route(Router* router, const char* method, const char* path, void (*handler)(HttpRequest*, HttpResponse*)) {
    if (router->count < MAX_ROUTES) {
        log_message(LOG_INFO, "Adding route: %s %s", method, path);
        router->routes[router->count].method = strdup(method);
        router->routes[router->count].path = strdup(path);
        router->routes[router->count].handler = handler;
        router->count++;
        log_message(LOG_INFO, "Route added successfully: %s %s", method, path);
    } else {
        log_message(LOG_WARN, "Failed to add route, max routes reached: %s %s", method, path);
    }
}

void router_handle_request(Router* router, HttpRequest* request, HttpResponse* response) {
    const char* method = http_request_get_method(request);
    const char* path = http_request_get_path(request);
    
    log_message(LOG_INFO, "Handling request: %s %s", method, path);
    for (size_t i = 0; i < router->count; i++) {
        if (strcmp(router->routes[i].method, method) == 0 && strcmp(router->routes[i].path, path) == 0) {
            log_message(LOG_INFO, "Route found, invoking handler: %s %s", method, path);
            router->routes[i].handler(request, response);
            log_message(LOG_INFO, "Handler executed successfully: %s %s", method, path);
            return;
        }
    }
    
    // 404 Not Found
    log_message(LOG_WARN, "Route not found, returning 404: %s %s", method, path);
    http_response_set_status(response, 404, "Not Found");
    http_response_set_body(response, "404 Not Found");
}

void router_destroy(Router* router) {
    log_message(LOG_INFO, "Destroying router");
    for (size_t i = 0; i < router->count; i++) {
        free(router->routes[i].method);
        free(router->routes[i].path);
    }
    free(router);
    log_message(LOG_INFO, "Router destroyed successfully");
}
