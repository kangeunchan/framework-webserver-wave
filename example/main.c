#include "../include/wave.h"

void helloHandler(Request* req, Response* res) {
    log(INFO, "helloHandler called");
    setBody(res, "Hello, World!");
    log(INFO, "helloHandler completed");
}

void echoHandler(Request* req, Response* res) {
    log(INFO, "echoHandler called");
    if (req->body == NULL) {
        log(ERROR, "req->body is NULL");
        setBody(res, "Echo: (no body)");
        log(INFO, "echoHandler completed with no body");
        return;
    }
    char* echo_message = malloc(strlen(req->body) + 7);
    if (echo_message == NULL) {
        log(ERROR, "Memory allocation failed in echoHandler");
        return;
    }
    sprintf(echo_message, "Echo: %s", req->body);
    setBody(res, echo_message);
    free(echo_message);
    log(INFO, "echoHandler completed");
}

int main() {
    log(INFO, "Application starting");

    Application* app = createApplication();
    if (app == NULL) {
        log(ERROR, "Failed to create application");
        return 1;
    }

    // Set up routes
    log(INFO, "Setting up routes");
    addRoute(app->router, GET, "/hello", helloHandler);
    addRoute(app->router, POST, "/echo", echoHandler);

    // Set up server info
    log(INFO, "Setting up server info");
    app->server_info.PORT = 8080;

    log(INFO, "Running application");
    runApplication(app);

    // Clean up
    log(INFO, "Cleaning up application");
    destroyApplication(app);

    log(INFO, "Application terminated");
    return 0;
}
