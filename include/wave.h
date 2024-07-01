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
#define MAX_PATH_LENGTH 256
#define MAX_JSON_FIELDS 20

typedef enum {
    GET,
    POST,
    PUT,
    DELETE,
    PATCH
} HttpMethod;

typedef struct {
    HttpMethod method;
    char path[MAX_PATH_LENGTH];
    char body[MAX_REQUEST_SIZE];
    char json_path[MAX_PATH_LENGTH];
} HttpRequest;

typedef struct {
    int status_code;
    char content[MAX_REQUEST_SIZE];
} HttpResponse;

typedef void (*RouteHandler)(HttpRequest*, HttpResponse*);

typedef struct {
    HttpMethod method;
    char path[MAX_PATH_LENGTH];
    RouteHandler handler;
} Route;

typedef void (*Middleware)(HttpRequest*, HttpResponse*, int*);

typedef struct {
    Route routes[MAX_ROUTE_COUNT];
    int route_count;
    Middleware middlewares[MAX_MIDDLEWARE_COUNT];
    int middleware_count;
} WebFramework;

typedef struct {
    char key[50];
    char value[256];
} JsonField;

typedef struct {
    JsonField fields[MAX_JSON_FIELDS];
    int field_count;
} JsonObject;

// Function declarations
void init_framework(WebFramework* framework);
void add_route(WebFramework* framework, HttpMethod method, const char* path, RouteHandler handler);
void add_middleware(WebFramework* framework, Middleware middleware);
void handle_request(WebFramework* framework, HttpRequest* request, HttpResponse* response);
void start_server(WebFramework* framework, int port);
void send_response(int client_socket, HttpResponse* response);
void parse_request(const char* request_str, HttpRequest* request);
const char* method_to_string(HttpMethod method);
void json_parser(const char* request_str, HttpRequest* request);
void json_to_string(const JsonObject* obj, char* output, size_t output_size);
void set_json_response_from_object(HttpResponse* response, int status_code, const JsonObject* json_obj);

#define JSON_ADD_STRING(obj, k, v) \
    do { \
        strncpy(obj.fields[obj.field_count].key, k, sizeof(obj.fields[obj.field_count].key) - 1); \
        obj.fields[obj.field_count].key[sizeof(obj.fields[obj.field_count].key) - 1] = '\0'; \
        strncpy(obj.fields[obj.field_count].value, v, sizeof(obj.fields[obj.field_count].value) - 1); \
        obj.fields[obj.field_count].value[sizeof(obj.fields[obj.field_count].value) - 1] = '\0'; \
        obj.field_count++; \
    } while(0)

#endif