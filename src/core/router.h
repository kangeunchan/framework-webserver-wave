#ifndef ROUTER_H
#define ROUTER_H

#include "http_request.h"
#include "http_response.h"

typedef struct Router Router;

Router* router_create();
void router_add_route(Router* router, const char* method, const char* path, void (*handler)(HttpRequest*, HttpResponse*));
void router_handle_request(Router* router, HttpRequest* request, HttpResponse* response);
void router_destroy(Router* router);

#endif