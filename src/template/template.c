#include "template.h"
#include <stdlib.h>
#include <string.h>

struct WaveTemplateContext {
    int a;
};

char* templateRender(const char *templateString, WaveTemplateContext *context) {
    return strdup(templateString);
}

WaveTemplateContext* templateContextCreate() {
    return (WaveTemplateContext*)malloc(sizeof(WaveTemplateContext));
}

void templateContextSet(WaveTemplateContext *ctx, const char *key, const char *value) {
}

void templateContextFree(WaveTemplateContext *ctx) {
    free(ctx);
}