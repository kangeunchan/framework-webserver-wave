#include "container.h"
#include "logger.h"
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
    log_message(LOG_INFO, "Creating container");
    Container* container = malloc(sizeof(Container));
    if (!container) {
        log_message(LOG_ERROR, "Failed to allocate memory for container");
        return NULL;
    }

    container->components = malloc(INITIAL_CAPACITY * sizeof(Component));
    if (!container->components) {
        log_message(LOG_ERROR, "Failed to allocate memory for components");
        free(container);
        return NULL;
    }

    container->count = 0;
    container->capacity = INITIAL_CAPACITY;
    log_message(LOG_INFO, "Container created successfully with initial capacity %d", INITIAL_CAPACITY);
    return container;
}

static int container_resize(Container* container) {
    size_t new_capacity = container->capacity * GROWTH_FACTOR;
    Component* new_components = realloc(container->components, new_capacity * sizeof(Component));
    if (!new_components) {
        log_message(LOG_ERROR, "Failed to reallocate memory for components");
        return 0;
    }

    container->components = new_components;
    container->capacity = new_capacity;
    log_message(LOG_INFO, "Container resized successfully to %zu", new_capacity);
    return 1;
}

int container_register(Container* container, const char* name, void* instance) {
    if (!container || !name || !instance) {
        log_message(LOG_ERROR, "Invalid arguments to container_register");
        return 0;
    }

    if (container->count == container->capacity && !container_resize(container)) {
        return 0;
    }

    for (size_t i = 0; i < container->count; i++) {
        if (strcmp(container->components[i].name, name) == 0) {
            free(container->components[i].name);
            container->components[i].name = strdup(name);
            container->components[i].instance = instance;
            log_message(LOG_INFO, "Component updated: %s", name);
            return 1;
        }
    }

    container->components[container->count].name = strdup(name);
    if (!container->components[container->count].name) {
        log_message(LOG_ERROR, "Failed to allocate memory for component name");
        return 0;
    }
    container->components[container->count].instance = instance;
    container->count++;
    log_message(LOG_INFO, "Component registered: %s", name);
    return 1;
}

void* container_resolve(Container* container, const char* name) {
    if (!container || !name) {
        log_message(LOG_ERROR, "Invalid arguments to container_resolve");
        return NULL;
    }

    for (size_t i = 0; i < container->count; i++) {
        if (strcmp(container->components[i].name, name) == 0) {
            log_message(LOG_INFO, "Component resolved: %s", name);
            return container->components[i].instance;
        }
    }

    log_message(LOG_WARN, "Component not found: %s", name);
    return NULL;
}

void container_destroy(Container* container) {
    if (container) {
        log_message(LOG_INFO, "Destroying container");
        for (size_t i = 0; i < container->count; i++) {
            free(container->components[i].name);
        }
        free(container->components);
        free(container);
        log_message(LOG_INFO, "Container destroyed successfully");
    }
}
