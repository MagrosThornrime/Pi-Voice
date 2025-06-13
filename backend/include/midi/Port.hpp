#pragma once

#include <string>
#include <stdexcept>
#include <format>
#include <Types.hpp>

namespace midi {

struct Port {
    const std::string_view name{};
    const u32 num = -1;

    constexpr operator bool() const noexcept {
        return num != (u32)-1 && name != std::string_view();
    }

    constexpr void assure() const {
        if(num == (u32)-1) {
            throw std::invalid_argument(std::format("Port '{}' not found", name));
        }
        else if(name == std::string_view()) {
            throw std::invalid_argument(std::format("Port {} not found", num));
        }
    }
};

}