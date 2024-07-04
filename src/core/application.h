#ifndef APPLICATION_H
#define APPLICATION_H

#include "router.h"
#include "server.h"

typedef struct {
    Router* router;
    ServerInfo server_info;
} Application;

Application* createApplication();
void runApplication(Application* app);
void destroyApplication(Application* app);

#endif // APPLICATION_H