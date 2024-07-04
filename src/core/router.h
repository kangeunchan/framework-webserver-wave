#ifndef ROUTER_H
#define ROUTER_H

#include "./request.h"
#include "./response.h"

typedef void (*RouteHandler)(Request*, Response*);

typedef struct {
    HttpMethod method;
    char* path;
    RouteHandler handler;
} Route;

typedef struct {
    Route* routes;
    int route_count;
    int max_routes;  // 추가된 필드
} Router;

Router* createRouter();
void addRoute(Router* router, HttpMethod method, const char* path, RouteHandler handler);
RouteHandler findRoute(Router* router, HttpMethod method, const char* path);
void destroyRouter(Router* router);

#endif // ROUTER_H