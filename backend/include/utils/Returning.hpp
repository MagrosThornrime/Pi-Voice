#pragma once

#include "CallableTraits.hpp"

namespace utils {


/// @brief Checks if type is callable and returns given type T
template<class Fn, class T>
concept Returning = std::same_as<T, typename CallableTraits<Fn>::Return>;

}