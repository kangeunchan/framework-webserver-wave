#ifndef WAVE_TEMPLATE_H
#define WAVE_TEMPLATE_H

typedef struct WaveTemplateContext WaveTemplateContext;

char* templateRender(const char *templateString, WaveTemplateContext *context);
WaveTemplateContext* templateContextCreate();
void templateContextSet(WaveTemplateContext *ctx, const char *key, const char *value);
void templateContextFree(WaveTemplateContext *ctx);

#endif // WAVE_TEMPLATE_H