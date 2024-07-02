#include "router.h"
#include <stdlib.h>
#include <string.h>

#define MAX_ROUTES 100

typedef struct {
    char path[100];
    char method[10];
    WaveHandler handler;
} Route;

static Route routes[MAX_ROUTES];
static int routeCount = 0;

void routerInit() {
    routeCount = 0;
}

void routerAddRoute(const char *path, const char *method, WaveHandler handler) {
    if (routeCount < MAX_ROUTES) {
        strncpy(routes[routeCount].path, path, sizeof(routes[routeCount].path) - 1);
        strncpy(routes[routeCount].method, method, sizeof(routes[routeCount].method) - 1);
        routes[routeCount].handler = handler;
        routeCount++;
    }
}

WaveHandler routerFindHandler(const char *path, const char *method) {
    for (int i = 0; i < routeCount; i++) {
        if (strcmp(routes[i].path, path) == 0 && strcmp(routes[i].method, method) == 0) {
            return routes[i].handler;
        }
    }
    return NULL;
}