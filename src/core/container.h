// container.h
#ifndef CONTAINER_H
#define CONTAINER_H

#include <stddef.h>

typedef struct Container Container;

Container* container_create(void);
int container_register(Container* container, const char* name, void* instance);
void* container_resolve(Container* container, const char* name);
void container_destroy(Container* container);

#endif