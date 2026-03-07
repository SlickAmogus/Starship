// Stub for Xbox build
#pragma once
#include "../spdlog.h"

namespace spdlog { namespace sinks {
class rotating_file_sink_mt : public sink {
public:
    rotating_file_sink_mt(const std::string&, size_t, size_t) {}
};
}}
