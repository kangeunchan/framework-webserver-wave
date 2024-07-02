#include "middleware.h"
#include <stdlib.h>

#define MAX_MIDDLEWARES 10

static WaveMiddlewareFunc middlewares[MAX_MIDDLEWARES];
static int middlewareCount = 0;

void middlewareAdd(WaveMiddlewareFunc func) {
    if (middlewareCount < MAX_MIDDLEWARES) {
        middlewares[middlewareCount++] = func;
    }
}

void middlewareExecute(WaveRequest *request, WaveResponse *response) {
    for (int i = 0; i < middlewareCount; i++) {
        middlewares[i](request, response);
    }
}