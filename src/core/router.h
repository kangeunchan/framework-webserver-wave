#ifndef ROUTER_H
#define ROUTER_H

#include "request.h"
#include "response.h"

void add_route(HttpMethod method, const char* path, Response* (*handler)(const Request*));
Response* handle_request(const Request* request);

#endif // ROUTER_H