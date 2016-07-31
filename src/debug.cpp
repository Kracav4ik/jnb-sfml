#include "debug.h"

#include <stdio.h>
#include <stdarg.h>
#include <SFML/System.hpp>

using namespace sf;

Clock log_clock;

void log(const char *fmt, ...) {
    Time elapsed = log_clock.getElapsedTime();
    va_list args;
    va_start(args, fmt);
    printf("[%6i] ", elapsed.asMilliseconds());
    vprintf(fmt, args);
    va_end(args);
}

