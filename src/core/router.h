#ifndef WAVE_ROUTER_H
#define WAVE_ROUTER_H

#include "request.h"
#include "response.h"

typedef void (*WaveHandler)(WaveRequest *request, WaveResponse *response);

void routerInit();
void routerAddRoute(const char *path, const char *method, WaveHandler handler);
WaveHandler routerFindHandler(const char *path, const char *method);

#endif // WAVE_ROUTER_H