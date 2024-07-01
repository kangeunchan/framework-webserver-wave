#ifndef WAVE_H
#define WAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_REQUEST_SIZE 4096
#define MAX_ROUTE_COUNT 100
#define MAX_MIDDLEWARE_COUNT 50

typedef enum {
    GET,
    POST,
    PUT,
    DELETE
} HttpMethod;

typedef struct {
    HttpMethod method;
    char path[256];
    char body[MAX_REQUEST_SIZE];
} HttpRequest;

typedef struct {
    int status_code;
    char content[MAX_REQUEST_SIZE];
} HttpResponse;

typedef void (*RouteHandler)(HttpRequest*, HttpResponse*);

typedef struct {
    HttpMethod method;
    char path[256];
    RouteHandler handler;
} Route;

typedef void (*Middleware)(HttpRequest*, HttpResponse*, int*);

typedef struct {
    Route routes[MAX_ROUTE_COUNT];
    int route_count;
    Middleware middlewares[MAX_MIDDLEWARE_COUNT];
    int middleware_count;
} WebFramework;

void init_framework(WebFramework* framework);
void add_route(WebFramework* framework, HttpMethod method, const char* path, RouteHandler handler);
void add_middleware(WebFramework* framework, Middleware middleware);
void handle_request(WebFramework* framework, HttpRequest* request, HttpResponse* response);
void start_server(WebFramework* framework, int port);

void send_response(int client_socket, HttpResponse* response);
void parse_request(char* request_str, HttpRequest* request);
const char* method_to_string(HttpMethod method);

#endif
