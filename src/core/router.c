#include "router.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    HttpMethod method;
    char* path;
    Response* (*handler)(const Request*);
} Route;

static Route* routes = NULL;
static int route_count = 0;

int routeAdd(HttpMethod method, const char* path, Response* (*handler)(const Request*)) {
    routes = realloc(routes, sizeof(Route) * (route_count + 1));
    if (routes == NULL) {
        return -1;  // 메모리 할당 실패
    }
    routes[route_count].method = method;
    routes[route_count].path = strdup(path);
    if (routes[route_count].path == NULL) {
        return -1;  // 메모리 할당 실패
    }
    routes[route_count].handler = handler;
    route_count++;
    return 0;  // 성공
}

Response* handle_request(const Request* request) {
    for (int i = 0; i < route_count; i++) {
        if (routes[i].method == request->method && strcmp(routes[i].path, request->path) == 0) {
            return routes[i].handler(request);
        }
    }
    return create_response(404, "404 Not Found");
}