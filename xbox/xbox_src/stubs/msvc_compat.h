// MSVC secure CRT function stubs for nxdk
// Include this from files that use vsnprintf_s, fopen_s, etc.
#pragma once

#include <stdio.h>
#include <stdarg.h>

#ifdef NXDK

#ifndef _XBOX_MSVC_COMPAT_DEFINED
#define _XBOX_MSVC_COMPAT_DEFINED

typedef int errno_t;

static inline int vsnprintf_s(char* buf, size_t sz, size_t count, const char* fmt, va_list args) {
    (void)count;
    return vsnprintf(buf, sz, fmt, args);
}

static inline int vsprintf_s(char* buf, const char* fmt, va_list args) {
    return vsprintf(buf, fmt, args);
}

static inline errno_t fopen_s(FILE** fp, const char* name, const char* mode) {
    *fp = fopen(name, mode);
    return (*fp == NULL) ? 1 : 0;
}

static inline int _vscprintf(const char* fmt, va_list args) {
    va_list copy;
    va_copy(copy, args);
    int ret = vsnprintf(NULL, 0, fmt, copy);
    va_end(copy);
    return ret;
}

#endif // _XBOX_MSVC_COMPAT_DEFINED
#endif // NXDK
