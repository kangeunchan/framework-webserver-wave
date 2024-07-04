#include "controller.h"

void hello_world(HttpRequest* request, HttpResponse* response) {
    http_response_set_status(response, 200, "OK");
    http_response_add_header(response, "Content-Type", "text/plain");
    http_response_set_body(response, "Hello, World!");
}