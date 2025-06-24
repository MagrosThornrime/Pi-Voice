#pragma once

#include <string>
#include <stdexcept>
#include <fmt/core.h>
#include <Types.hpp>

namespace midi {

/// @brief Midi port
struct Port {
	/// @brief Port name
    const std::string_view name{};
	/// @brief Port number
    const u32 num = -1;

    /// @brief Checks if port is valid
    constexpr operator bool() const noexcept {
        return num != (u32)-1 && name != std::string_view();
    }

	/// @brief Throws if port is invalid
    constexpr void assure() const {
        if(num == (u32)-1) {
            throw std::invalid_argument(fmt::format("Port '{}' not found", name));
        }
        else if(name == std::string_view()) {
            throw std::invalid_argument(fmt::format("Port {} not found", num));
        }
    }
};

}