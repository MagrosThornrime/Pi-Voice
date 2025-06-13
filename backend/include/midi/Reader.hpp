#pragma once

#include "InternalReader.hpp"
#include <mutex>
#include <tuple>

namespace midi {

class Reader {
public:

	Reader() noexcept = default;
	Reader(const Port port);

	void open(const Port port);
	void close() noexcept;

	Data read();
	std::vector<Data> readSome(const u32 n);
	std::vector<Data> readAll();
	u32 queueSize() const;
	void clear() noexcept;

	void setCallback(std::function<void(InternalReader&)> callback) noexcept;
	void resetCallback() noexcept;

	bool opened() const noexcept;
	operator bool() const noexcept;
	
	std::tuple<std::unique_lock<std::mutex>, InternalReader&> locked() noexcept;
	std::tuple<std::unique_lock<std::mutex>, const InternalReader&> locked() const noexcept;

private:

	midi::InternalReader _reader;
};

}