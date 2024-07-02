#include "template.h"
#include <stdio.h>
#include <stdlib.h>

char* render_template(const char* template_name, const char* data) {
    char* result = malloc(100);
    snprintf(result, 100, "Rendered template: %s with data: %s", template_name, data);
    return result;
}