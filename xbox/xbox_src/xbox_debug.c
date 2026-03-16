// Xbox debug logging implementation
#include <stdio.h>
#include <stdarg.h>
#include "xbox_debug.h"

void xbox_log(const char* fmt, ...) {
    // First call truncates (fresh log per run), subsequent calls append
    static int first = 1;
    FILE* f = fopen("D:\\starship.log", first ? "w" : "a");
    first = 0;
    if (f) {
        va_list args;
        va_start(args, fmt);
        vfprintf(f, fmt, args);
        va_end(args);
        fflush(f);
        fclose(f);
    }
}
