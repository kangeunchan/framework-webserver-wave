#include "wave.h"

void init_framework(WebFramework* framework) {
    if (!framework) return;
    
    framework->route_count = 0;
    framework->middleware_count = 0;
}

void add_route(WebFramework* framework, HttpMethod method, const char* path, RouteHandler handler) {
    if (!framework || !path || !handler) return;
    
    if (framework->route_count < MAX_ROUTE_COUNT) {
        Route* new_route = &framework->routes[framework->route_count];
        new_route->method = method;
        strncpy(new_route->path, path, sizeof(new_route->path) - 1);
        new_route->path[sizeof(new_route->path) - 1] = '\0';
        new_route->handler = handler;
        framework->route_count++;
    }
}

void add_middleware(WebFramework* framework, Middleware middleware) {
    if (!framework || !middleware) return;
    
    if (framework->middleware_count < MAX_MIDDLEWARE_COUNT) {
        framework->middlewares[framework->middleware_count] = middleware;
        framework->middleware_count++;
    }
}