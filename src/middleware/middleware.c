#include "middleware.h"
#include <stdlib.h>

static Middleware* middlewares = NULL;
static int middleware_count = 0;

void use_middleware(Middleware middleware) {
    middlewares = realloc(middlewares, sizeof(Middleware) * (middleware_count + 1));
    middlewares[middleware_count] = middleware;
    middleware_count++;
}

void execute_middlewares() {
    for (int i = 0; i < middleware_count; i++) {
        middlewares[i]();
    }
}
