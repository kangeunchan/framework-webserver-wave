#include "router.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>

#define MAX_ROUTES 100

typedef enum {
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_PATCH,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_TRACE,
    HTTP_CONNECT,
    HTTP_METHOD_COUNT
} HttpMethod;

typedef struct {
    HttpMethod method;
    char* path;
    void (*handler)(HttpRequest*, HttpResponse*);
} Route;

struct Router {
    Route routes[MAX_ROUTES];
    size_t count;
};

static const char* HTTP_METHOD_STRINGS[] = {
    "GET", "POST", "PUT", "DELETE", "PATCH", "HEAD", "OPTIONS", "TRACE", "CONNECT"
};

static HttpMethod string_to_http_method(const char* method) {
    for (int i = 0; i < HTTP_METHOD_COUNT; i++) {
        if (strcmp(method, HTTP_METHOD_STRINGS[i]) == 0) {
            return (HttpMethod)i;
        }
    }
    return HTTP_METHOD_COUNT; // Invalid method
}

Router* router_create() {
    Router* router = calloc(1, sizeof(Router));
    if (!router) {
        log_message(LOG_ERROR, "Failed to allocate memory for router");
        return NULL;
    }
    log_message(LOG_INFO, "Router created successfully");
    return router;
}

void router_add_route(Router* router, const char* method, const char* path, void (*handler)(HttpRequest*, HttpResponse*)) {
    if (router->count >= MAX_ROUTES) {
        log_message(LOG_WARN, "Failed to add route, max routes reached: %s %s", method, path);
        return;
    }
    
    HttpMethod http_method = string_to_http_method(method);
    if (http_method == HTTP_METHOD_COUNT) {
        log_message(LOG_WARN, "Invalid HTTP method: %s", method);
        return;
    }

    router->routes[router->count].method = http_method;
    router->routes[router->count].path = strdup(path);
    router->routes[router->count].handler = handler;
    router->count++;
    log_message(LOG_INFO, "Route added successfully: %s %s", method, path);
}

void router_handle_request(Router* router, HttpRequest* request, HttpResponse* response) {
    const char* method_str = http_request_get_method(request);
    const char* path = http_request_get_path(request);
    HttpMethod method = string_to_http_method(method_str);
    
    for (size_t i = 0; i < router->count; i++) {
        if (router->routes[i].method == method && strcmp(router->routes[i].path, path) == 0) {
            router->routes[i].handler(request, response);
            log_message(LOG_INFO, "Handler executed for: %s %s", method_str, path);
            return;
        }
    }

    http_response_set_status(response, 404, "Not Found");
    http_response_set_body(response, "404 Not Found");
    log_message(LOG_WARN, "No route matched, returning 404 for: %s %s", method_str, path);
}

void router_destroy(Router* router) {
    for (size_t i = 0; i < router->count; i++) {
        free(router->routes[i].path);
    }
    free(router);
    log_message(LOG_INFO, "Router destroyed");
}
