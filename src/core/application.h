// application.h
#ifndef APPLICATION_H
#define APPLICATION_H

#include "container.h"
#include "http_server.h"
#include "router.h"

typedef struct Application {
    Container* container;
    HttpServer* http_server;
    Router* router;
} Application;

Application* application_create(void);
void application_run(Application* app);
void application_destroy(Application* app);

#endif

