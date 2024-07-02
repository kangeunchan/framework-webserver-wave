#include "include/wave.h"

void homeHandler(WaveRequest *request, WaveResponse *response) {
    responseSetBody(response, "<h1>Welcome to Wave Framework!</h1>");
}

void aboutHandler(WaveRequest *request, WaveResponse *response) {
    responseSetBody(response, "<h1>About Wave Framework</h1><p>A simple web framework in C.</p>");
}

void templateHandler(WaveRequest *request, WaveResponse *response) {
    WaveTemplateContext *context = templateContextCreate();
    templateContextSet(context, "title", "Template Example");
    templateContextSet(context, "content", "This is a rendered template.");

    const char *templateString = "<h1>{{title}}</h1><p>{{content}}</p>";
    char *renderedHtml = templateRender(templateString, context);

    responseSetBody(response, renderedHtml);

    free(renderedHtml);
    templateContextFree(context);
}

void loggingMiddleware(WaveRequest *request, WaveResponse *response) {
    char logMessage[256];
    snprintf(logMessage, sizeof(logMessage), "Request received: %s %s", requestGetMethod(request), requestGetPath(request));
    logInfo(logMessage);
}

int main() {
    configLoad("config.ini");
    int port = configGetInt("server_port");
    if (port == 0) port = 8080;

    logInit("server.log");

    dbConnect("mysql://localhost/wavedb");

    routerInit();
    routerAddRoute("/", "GET", homeHandler);
    routerAddRoute("/about", "GET", aboutHandler);
    routerAddRoute("/template", "GET", templateHandler);

    middlewareAdd(loggingMiddleware);

    serverInit(port);
    
    printf("Wave Framework server starting on port %d\n", port);
    logInfo("Server starting");

    serverStart();

    logInfo("Server shutting down");
    dbClose();

    return 0;
}