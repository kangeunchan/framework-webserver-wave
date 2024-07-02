#ifndef WAVE_DISPATCHER_H
#define WAVE_DISPATCHER_H

#include "request.h"
#include "response.h"

void serverDispatch(WaveRequest *request, WaveResponse *response);

#endif // WAVE_DISPATCHER_H