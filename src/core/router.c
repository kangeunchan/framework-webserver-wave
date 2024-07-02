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

void routeAdd(HttpMethod method, const char* path, Response* (*handler)(const Request*)) {
    routes = realloc(routes, sizeof(Route) * (route_count + 1));
    routes[route_count].method = method;
    routes[route_count].path = strdup(path);
    routes[route_count].handler = handler;
    route_count++;
}

Response* handle_request(const Request* request) {
    for (int i = 0; i < route_count; i++) {
        if (routes[i].method == request->method && strcmp(routes[i].path, request->path) == 0) {
            return routes[i].handler(request);
        }
    }
    return create_response(404, "404 Not Found");
}