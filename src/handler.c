#include "wave.h"

void handle_request(WebFramework* framework, HttpRequest* request, HttpResponse* response) {
    int middleware_index = 0;
    for (int i = 0; i < framework->middleware_count; i++) {
        framework->middlewares[i](request, response, &middleware_index);
        if (middleware_index != i + 1) {
            return;
        }
    }

    for (int i = 0; i < framework->route_count; i++) {
        if (framework->routes[i].method == request->method &&
            strcmp(framework->routes[i].path, request->path) == 0) {
            framework->routes[i].handler(request, response);
            return;
        }
    }

    response->status_code = 404;
    strcpy(response->content, "Not Found");
}
