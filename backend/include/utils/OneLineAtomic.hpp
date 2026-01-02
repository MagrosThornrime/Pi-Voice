#pragma once

#include <atomic>
#include <new>

namespace utils {

template<class T>
struct alignas(64) OneLineAtomic {
	std::atomic<T> atomic;
	char _padding[64 - sizeof(atomic)];
};

}