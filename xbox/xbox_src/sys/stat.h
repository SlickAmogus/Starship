// Minimal sys/stat.h stub for nxdk
// miniz needs struct __stat64 for file size queries
#pragma once
#include <stdint.h>

struct __stat64 {
    int64_t st_size;
    int64_t st_mtime;
    unsigned st_mode;
};

struct _stat {
    int64_t st_size;
    int64_t st_mtime;
    unsigned st_mode;
};
