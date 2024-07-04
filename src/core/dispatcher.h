#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "router.h"
#include "request.h"
#include "response.h"

void dispatch(Router* router, Request* request, Response* response);

#endif // DISPATCHER_H