#pragma once

#include "CallableTraits.hpp"

namespace utils {

/// @brief Checks if type is callable
template<class Fn>
concept Callable = CallableTraits<Fn>::isCallable;

}