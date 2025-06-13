#pragma once

#include <RtMidi.h>
#include <Types.hpp>
#include <unordered_map>
#include <mutex>
#include <concepts>
#include "Data.hpp"
#include "Port.hpp"
#include <utils/Returning.hpp>
#include <utils/Callable.hpp>

namespace midi {

class Ports {
public:

    static std::unique_lock<std::mutex> lock() noexcept;
    template<utils::Callable Fn, class... Args>
    static auto locked(Fn&& fn, Args&&... args) noexcept(noexcept(fn(std::forward<Args>(args)...))) -> decltype(fn(std::forward<Args>(args)...)) { auto _ = lock(); return fn(std::forward<Args>(args)...); }
    template<utils::Returning<void> Fn, class... Args>
    static void locked(Fn&& fn, Args&&... args) noexcept(noexcept(fn(std::forward<Args>(args)...))) { auto _ = lock(); fn(std::forward<Args>(args)...); }

    static void refresh() noexcept;
    
    static std::vector<Port> list() noexcept;
    static u32 count() noexcept;

    static Port getByName(std::string_view name);
    static Port getByNum(const u32 num);

    static Port get(std::string_view name);
    static Port get(const u32 num);

private:

    static std::mutex _mutex;
    static std::unordered_map<std::string_view, u32> _numbers;
    static std::vector<std::string> _names;
};

}