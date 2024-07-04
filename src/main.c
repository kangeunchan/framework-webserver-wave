#include "core/application.h"
#include "web/controller.h"
#include "core/logger.h"

int main() {
    
    log_init("./logs/app.log");

    Application* app = application_create();
    


    router_add_route(app->router, "GET", "/", hello_world);
    
    application_run(app);
    application_destroy(app);
    
    return 0;
}