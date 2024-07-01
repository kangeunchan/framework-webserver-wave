#include "wave.h"

void init_framework(WebFramework* framework) {
    framework->route_count = 0;
    framework->middleware_count = 0;
}

void add_route(WebFramework* framework, HttpMethod method, const char* path, RouteHandler handler) {
    if (framework->route_count < MAX_ROUTE_COUNT) {
        framework->routes[framework->route_count].method = method;
        strncpy(framework->routes[framework->route_count].path, path, sizeof(framework->routes[framework->route_count].path) - 1);
        framework->routes[framework->route_count].handler = handler;
        framework->route_count++;
    }
}

void add_middleware(WebFramework* framework, Middleware middleware) {
    if (framework->middleware_count < MAX_MIDDLEWARE_COUNT) {
        framework->middlewares[framework->middleware_count] = middleware;
        framework->middleware_count++;
    }
}
