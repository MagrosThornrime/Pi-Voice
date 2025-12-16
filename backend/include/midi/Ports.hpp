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

/// @brief Utility class for managing midi port database
class Ports {
public:

	/// @brief Returns lock to database
	static std::unique_lock<std::mutex> lock() noexcept;
	/// @brief Calls given callable with aquired lock and returns its value
	/// @param fn callable to call
	/// @param args... args to pass to fn
	/// @return fn(args...)
	template<utils::Callable Fn, class... Args>
	static auto locked(Fn&& fn, Args&&... args) noexcept(noexcept(fn(std::forward<Args>(args)...))) -> decltype(fn(std::forward<Args>(args)...)) { auto _ = lock(); return fn(std::forward<Args>(args)...); }
	/// @brief Calls given callable with aquired lock
	/// @param fn callable to call
	/// @param args... args to pass to fn
	template<utils::Returning<void> Fn, class... Args>
	static void locked(Fn&& fn, Args&&... args) noexcept(noexcept(fn(std::forward<Args>(args)...))) { auto _ = lock(); fn(std::forward<Args>(args)...); }

	/// @brief Refreshes database
	static void refresh() noexcept;

	/// @brief Returns vector of present ports
	static std::vector<Port> list() noexcept;
	/// @brief Returns count of ports
	static u32 count() noexcept;

	/// @brief Returns port by name
	/// @param name name of port to find
	static Port getByName(const std::string& name);
	/// @brief Returns port by number
	/// @param num number of port to find
	static Port getByNum(const u32 num);

	/// @brief Returns port by name
	/// @param name name of port to find
	static Port get(const std::string& name);
	/// @brief Returns port by number
	/// @param num number of port to find
	static Port get(const u32 num);

private:

	static std::mutex _mutex;
	static std::unordered_map<std::string, u32> _numbers;
	static std::vector<std::string> _names;
};

}