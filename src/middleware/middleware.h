#ifndef WAVE_MIDDLEWARE_H
#define WAVE_MIDDLEWARE_H

#include "../core/request.h"
#include "../core/response.h"

typedef void (*WaveMiddlewareFunc)(WaveRequest *request, WaveResponse *response);

void middlewareAdd(WaveMiddlewareFunc func);
void middlewareExecute(WaveRequest *request, WaveResponse *response);

#endif // WAVE_MIDDLEWARE_H