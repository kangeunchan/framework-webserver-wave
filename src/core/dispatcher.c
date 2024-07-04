#include "../include/wave.h"

void dispatch(Router* router, Request* request, Response* response) {
    if (router == NULL || request == NULL || response == NULL) {
        log(ERROR, "DISPATCH NULL POINTER ERROR");
        return;
    }

    RouteHandler handler = findRoute(router, request->method, request->path);
    if (handler) {
        handler(request, response);
    } else {
        setStatusCode(response, 404);
        setBody(response, "404 Not Found");
    }
}
