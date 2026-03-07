// Stub for Xbox build
#pragma once

#include <cstdio>
#include <string>

namespace fmt {
    template<typename... Args>
    inline std::string format(const char* fmt, Args&&... args) {
        char buf[256];
        snprintf(buf, sizeof(buf), fmt);
        return buf;
    }
}
