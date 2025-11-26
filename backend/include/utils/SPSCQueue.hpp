#pragma once

#include <Types.hpp>
#include <memory>
#include <bit>
#include <optional>
#include "OneLineAtomic.hpp"

namespace utils {

template<class T>
class SPSCQueue {
public:
	SPSCQueue(const u32 n);
	~SPSCQueue();

	bool push(const T& value);
	bool push(T&& value);
	u32 push(const T* arr, u32 n);

	bool pop();
	bool pop(T& dst);
	u32 pop(T* dstArr, u32 n);
	std::optional<T> popOpt();

	u32 size() const;
	u32 capacity() const;
	bool full() const;

private:
	struct Data {
		alignas(T) u8 bytes[sizeof(T)];
	};

	T* getPtr(const u32 i = 0);

	u32 _capacity;
	u32 _mask;
	std::unique_ptr<Data[]> _array;
	OneLineAtomic<u32> _head;
	OneLineAtomic<u32> _tail;
};

template<class T>
T* SPSCQueue<T>::getPtr(const u32 i) {
	return reinterpret_cast<T*>((_array.get() + i)->bytes);
}

template<class T>
SPSCQueue<T>::SPSCQueue(const u32 n):
	_capacity{std::bit_ceil(n)},
	_mask{_capacity - 1},
	_array{std::make_unique<Data[]>(_capacity)},
	_head{0},
	_tail{0} {}

template<class T>
SPSCQueue<T>::~SPSCQueue() {
	while (pop());
}

template<class T>
u32 SPSCQueue<T>::size() const {
	return _tail.atomic.load(std::memory_order_acquire) - _head.atomic.load(std::memory_order_acquire);
}

template<class T>
u32 SPSCQueue<T>::capacity() const {
	return _capacity;
}

template<class T>
bool SPSCQueue<T>::full() const {
	return (_tail.atomic.load(std::memory_order_acquire) - _head.atomic.load(std::memory_order_acquire)) == capacity();
}

template<class T>
bool SPSCQueue<T>::push(const T& value) {
	auto tail = _tail.atomic.load(std::memory_order_relaxed);
	auto head = _head.atomic.load(std::memory_order_acquire);
	if (tail - head == _capacity) {
		return false;
	}

	new(getPtr() + (tail & _mask)) T(value);

	_tail.atomic.store(tail + 1, std::memory_order_release);

	return true;
}

template<class T>
bool SPSCQueue<T>::push(T&& value) {
	auto tail = _tail.atomic.load(std::memory_order_relaxed);
	auto head = _head.atomic.load(std::memory_order_acquire);
	if (tail - head == _capacity) {
		return false;
	}

	new(getPtr() + (tail & _mask)) T(std::forward<T>(value));

	_tail.atomic.store(tail + 1, std::memory_order_release);

	return true;
}

template<class T>
u32 SPSCQueue<T>::push(const T* arr, u32 n) {
	if (not arr) {
		return 0;
	}

	auto tail = _tail.atomic.load(std::memory_order_relaxed);
	auto head = _head.atomic.load(std::memory_order_acquire);

	n = std::min(n, _capacity - (tail - head));
	auto ptr = getPtr();
	for (u32 i = 0; i != n; ++i, ++tail) {
		new(ptr + (tail & _mask)) T(arr[i]);
	}

	_tail.atomic.store(tail, std::memory_order_release);

	return n;
}

template<class T>
u32 SPSCQueue<T>::pop(T* dstArr, u32 n) {
	auto head = _head.atomic.load(std::memory_order_relaxed);
	auto tail = _tail.atomic.load(std::memory_order_acquire);

	if (head == tail) {
		return 0;
	}

	n = std::min(n, tail - head);

	if (dstArr) {
		auto ptr = getPtr();
		for (u32 i = 0; i != n; ++i) {
			dstArr[i] = std::move(ptr[(head + i) & _mask]);
		}
	}
	auto ptr = getPtr();
	for (u32 i = 0; i != n; ++i, ++head) {
		ptr[head & _mask].~T();
	}

	_head.atomic.store(head, std::memory_order_release);

	return n;
}

template<class T>
bool SPSCQueue<T>::pop(T& dst) {
	return pop(&dst, 1);
}

template<class T>
bool SPSCQueue<T>::pop() {
	return pop(nullptr, 1);
}

template<class T>
std::optional<T> SPSCQueue<T>::popOpt() {
	std::optional<T> result = std::nullopt;

	auto head = _head.atomic.load(std::memory_order_relaxed);
	auto tail = _tail.atomic.load(std::memory_order_acquire);

	if (head == tail) {
		return result;
	}

	auto ptr = getPtr();
	result.emplace(std::move(ptr[head & _mask]));
	ptr[head & _mask].~T();

	_head.atomic.store(head + 1, std::memory_order_release);

	return result;
}

}