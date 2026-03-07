// Stub: BS::thread_pool for Xbox/nxdk (no threading support)
// Tasks are executed synchronously on the calling thread.
#pragma once

#include <functional>
#include <future>
#include <cstddef>

namespace BS {

// Priority type stub
using priority_t = int;
namespace pr {
    constexpr priority_t highest = 0;
    constexpr priority_t high = 1;
    constexpr priority_t normal = 2;
    constexpr priority_t low = 3;
    constexpr priority_t lowest = 4;
}

class thread_pool {
public:
    thread_pool(size_t = 1) {}

    // Submit with priority (ignored - runs synchronously)
    template <typename F, typename... Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using return_type = decltype(f(args...));
        std::promise<return_type> promise;
        auto future = promise.get_future();
        try {
            if constexpr (std::is_void_v<return_type>) {
                f(std::forward<Args>(args)...);
                promise.set_value();
            } else {
                promise.set_value(f(std::forward<Args>(args)...));
            }
        } catch (...) {
            promise.set_exception(std::current_exception());
        }
        return future;
    }

    // Submit with priority parameter
    template <typename F, typename... Args>
    auto submit(priority_t, F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        return submit(std::forward<F>(f), std::forward<Args>(args)...);
    }

    // submit_task - same as submit but takes callable only (no extra args)
    template <typename F>
    auto submit_task(F&& f) -> std::future<decltype(f())> {
        return submit(std::forward<F>(f));
    }

    template <typename F>
    auto submit_task(F&& f, priority_t) -> std::future<decltype(f())> {
        return submit(std::forward<F>(f));
    }

    void wait_for_tasks() {}
    void pause() {}
    void unpause() {}
    size_t get_thread_count() const { return 1; }
};

} // namespace BS
