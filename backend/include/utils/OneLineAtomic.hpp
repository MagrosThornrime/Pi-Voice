#pragma once

#include <atomic>
#include <new>

namespace utils {

template<class T>
struct alignas(std::hardware_destructive_interference_size) OneLineAtomic {
	std::atomic<T> atomic;
	char _padding[std::hardware_destructive_interference_size - sizeof(atomic)];
};

}