#pragma once

#include "CallableTraits.hpp"

namespace utils {

template<class Fn>
concept Callable = CallableTraits<Fn>::isCallable;

}