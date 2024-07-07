#include "wave.h"
#include <stdio.h>

int visitNum = 0;

void visit(HttpRequest* req, HttpResponse* res) {
    visitNum++;
    char * message;
    snprintf(message, 2, "hi");
    http_response_set_body(res, message);
}

int main() {

    log_init("./logs/app.log");

    Application* app = application_create();

    router_add_route(app->router, "GET", "/visit", visit);

    application_run(app);
    application_destroy(app);
    
    return 0;
}