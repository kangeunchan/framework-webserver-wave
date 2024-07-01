#include "wave.h"

void hello_handler(HttpRequest* req, HttpResponse* res) {
    res->status_code = 200;
    strcpy(res->content, "Hello, World!");
}

int main() {
    WebFramework framework;
    init_framework(&framework);

    add_route(&framework, GET, "/hello", hello_handler);

    start_server(&framework, 8080);

    return 0;
}
