#include "container.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2

typedef struct {
    char* name;
    void* instance;
} Component;

struct Container {
    Component* components;
    size_t count;
    size_t capacity;
};

Container* container_create(void) {
    Container* container = malloc(sizeof(Container));
    if (!container) return NULL;

    container->components = malloc(INITIAL_CAPACITY * sizeof(Component));
    if (!container->components) {
        free(container);
        return NULL;
    }

    container->count = 0;
    container->capacity = INITIAL_CAPACITY;

    return container;
}

static int container_resize(Container* container) {
    size_t new_capacity = container->capacity * GROWTH_FACTOR;
    Component* new_components = realloc(container->components, new_capacity * sizeof(Component));
    if (!new_components) return 0;

    container->components = new_components;
    container->capacity = new_capacity;
    return 1;
}

int container_register(Container* container, const char* name, void* instance) {
    if (!container || !name || !instance) return 0;

    if (container->count == container->capacity) {
        if (!container_resize(container)) return 0;
    }

    container->components[container->count].name = strdup(name);
    if (!container->components[container->count].name) return 0;

    container->components[container->count].instance = instance;
    container->count++;

    return 1;
}

void* container_resolve(Container* container, const char* name) {
    if (!container || !name) return NULL;

    for (size_t i = 0; i < container->count; i++) {
        if (strcmp(container->components[i].name, name) == 0) {
            return container->components[i].instance;
        }
    }

    return NULL;
}

void container_destroy(Container* container) {
    if (container) {
        for (size_t i = 0; i < container->count; i++) {
            free(container->components[i].name);
        }
        free(container->components);
        free(container);
    }
}
