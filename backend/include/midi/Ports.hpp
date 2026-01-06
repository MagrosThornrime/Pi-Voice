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
	/// @brief Refreshes database
	static void refresh() noexcept;

	/// @brief Returns vector of present ports
	static std::vector<Port> list() noexcept;
	/// @brief Returns count of ports
	static u32 count() noexcept;

	/// @brief Returns port by name
	/// @param name name of port to find
	static Port getByName(std::string_view name);
	/// @brief Returns port by number
	/// @param num number of port to find
	static Port getByNum(const u32 num);

	/// @brief Returns port by name
	/// @param name name of port to find
	static Port get(std::string_view name);
	/// @brief Returns port by number
	/// @param num number of port to find
	static Port get(const u32 num);

private:

	static std::mutex _mutex;
	static std::unordered_map<std::string_view, u32> _numbers;
	static std::vector<std::string> _names;
};

}