#ifndef WAVE_H
#define WAVE_H

#ifndef _STDIO_H_
#include <stdio.h>
#endif
#ifndef _STDLIB_H_
#include <stdlib.h>
#endif
#ifndef _STRING_H_
#include <string.h>
#endif
#ifndef _SYS_ERRNO_H_
#include <errno.h>
#endif
#ifndef _LIMITS_H_
#include <limits.h>
#endif
#ifndef _UNISTD_H_
#include <unistd.h>
#endif

#include "../src/core/application.h"
#include "../src/core/dispatcher.h"
#include "../src/core/request.h"
#include "../src/core/response.h"
#include "../src/core/router.h"
#include "../src/core/server.h"
#include "../src/utils/logger.h"
#include "../src/config/settings.h"

#endif // WAVE_H