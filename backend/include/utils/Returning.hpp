#pragma once

#include "CallableTraits.hpp"

namespace utils {

template<class Fn, class T>
concept Returning = std::same_as<T, typename CallableTraits<Fn>::Return>;

}