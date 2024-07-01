#include "wave.h"

void hello_handler(HttpRequest* request, HttpResponse* response) {
    response->status_code = 200;
    strcpy(response->content, "Hello, World!");
}

void echo_handler(HttpRequest* request, HttpResponse* response) {
    response->status_code = 200;
    snprintf(response->content, sizeof(response->content), "You said: %s", request->body);
}

void logging_middleware(HttpRequest* request, HttpResponse* response, int* next) {
    printf("Received %s request for %s\n", method_to_string(request->method), request->path);
    *next = *next + 1;
}

int main() {
    WebFramework framework;
    init_framework(&framework);

    add_middleware(&framework, logging_middleware);
    add_route(&framework, GET, "/hello", hello_handler);
    add_route(&framework, POST, "/echo", echo_handler);

    printf("Starting server on port 8080...\n");
    start_server(&framework, 8080);

    return 0;
}