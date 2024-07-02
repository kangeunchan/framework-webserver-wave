#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

typedef void (*Middleware)(void);

void use_middleware(Middleware middleware);
void execute_middlewares();

#endif // MIDDLEWARE_H
