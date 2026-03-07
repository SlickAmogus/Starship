// Minimal spdlog stub for Xbox build
// Replaces spdlog with debugPrint or no-ops
#pragma once

#include <cstdio>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

// Stub out all spdlog macros to no-ops
#define SPDLOG_TRACE(...)    ((void)0)
#define SPDLOG_DEBUG(...)    ((void)0)
#define SPDLOG_INFO(...)     ((void)0)
#define SPDLOG_WARN(...)     ((void)0)
#define SPDLOG_ERROR(...)    ((void)0)
#define SPDLOG_CRITICAL(...) ((void)0)

#define SPDLOG_LOGGER_TRACE(logger, ...)    ((void)0)
#define SPDLOG_LOGGER_DEBUG(logger, ...)    ((void)0)
#define SPDLOG_LOGGER_INFO(logger, ...)     ((void)0)
#define SPDLOG_LOGGER_WARN(logger, ...)     ((void)0)
#define SPDLOG_LOGGER_ERROR(logger, ...)    ((void)0)
#define SPDLOG_LOGGER_CRITICAL(logger, ...) ((void)0)

#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL 6
#endif

#ifndef SPDLOG_FUNCTION
#define SPDLOG_FUNCTION __FUNCTION__
#endif

namespace spdlog {
    inline void set_level(int) {}
    inline void set_pattern(const char*) {}

    namespace level {
        enum level_enum { trace, debug, info, warn, err, critical, off };
    }

    namespace async_overflow_policy {
        enum overrun_oldest_t { block, overrun_oldest };
    }

    struct source_loc {
        const char* filename;
        int line;
        const char* funcname;
    };

    class spdlog_ex : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    // Sink base
    class sink {
    public:
        virtual ~sink() = default;
        void set_level(level::level_enum) {}
    };
    using sink_ptr = std::shared_ptr<sink>;

    class logger {
    public:
        logger() = default;
        logger(const std::string&) {}
        template<typename It>
        logger(const std::string&, It, It) {}
        void set_level(level::level_enum) {}
        void set_pattern(const std::string&) {}
        void flush_on(level::level_enum) {}
        template<typename... Args> void trace(Args&&...) {}
        template<typename... Args> void debug(Args&&...) {}
        template<typename... Args> void info(Args&&...) {}
        template<typename... Args> void warn(Args&&...) {}
        template<typename... Args> void error(Args&&...) {}
        template<typename... Args> void critical(Args&&...) {}
        template<typename... Args> void log(source_loc, level::level_enum, Args&&...) {}
        template<typename... Args> void log(level::level_enum, Args&&...) {}
    };

    class async_logger : public logger {
    public:
        template<typename It, typename... Args>
        async_logger(const std::string& name, It begin, It end, Args&&...) : logger(name, begin, end) {}
    };

    // Thread pool stub
    class thread_pool_stub {};
    inline void init_thread_pool(size_t, size_t) {}
    inline std::shared_ptr<thread_pool_stub> thread_pool() { return std::make_shared<thread_pool_stub>(); }

    inline std::shared_ptr<logger> get(const std::string&) { return std::make_shared<logger>(); }
    inline std::shared_ptr<logger> default_logger() { return std::make_shared<logger>(); }
    inline logger* default_logger_raw() { static logger l; return &l; }
    inline void register_logger(std::shared_ptr<logger>) {}
    inline void set_default_logger(std::shared_ptr<logger>) {}
    inline void shutdown() {}
}
