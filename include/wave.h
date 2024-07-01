#ifndef WAVE_H
#define WAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define MAX_REQUEST_SIZE 1024
#define MAX_ROUTE_COUNT 100

typedef enum {
    GET,
    POST
} HttpMethod;

typedef struct {
    HttpMethod method;
    char path[256];
} HttpRequest;

typedef struct {
    int status_code;
    char content[1024];
} HttpResponse;

typedef void (*RouteHandler)(HttpRequest*, HttpResponse*);

typedef struct {
    HttpMethod method;
    char path[256];
    RouteHandler handler;
} Route;

typedef struct {
    Route routes[MAX_ROUTE_COUNT];
    int route_count;
} WebFramework;

void init_framework(WebFramework* framework);
void add_route(WebFramework* framework, HttpMethod method, const char* path, RouteHandler handler);
void handle_request(WebFramework* framework, HttpRequest* request, HttpResponse* response);
void start_server(WebFramework* framework, int port);

#endif
