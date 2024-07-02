#include "dispatcher.h"
#include "router.h"
#include "../middleware/middleware.h"

void serverDispatch(WaveRequest *request, WaveResponse *response) {
    middlewareExecute(request, response);

    WaveHandler handler = routerFindHandler(requestGetPath(request), requestGetMethod(request));
    if (handler) {
        handler(request, response);
    } else {
        responseSetStatus(response, 404);
        responseSetBody(response, "404 Not Found");
    }
}