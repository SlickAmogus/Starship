// Xbox debug logging implementation
#include <stdio.h>
#include <stdarg.h>
#include "xbox_debug.h"

void xbox_log(const char* fmt, ...) {
    FILE* f = fopen("D:\\starship.log", "a");
    if (f) {
        va_list args;
        va_start(args, fmt);
        vfprintf(f, fmt, args);
        va_end(args);
        fflush(f);
        fclose(f);
    }
}
