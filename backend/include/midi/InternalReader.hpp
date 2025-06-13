#pragma once

#include <RtMidi.h>
#include <Types.hpp>
#include <deque>
#include <mutex>
#include <functional>
#include "Data.hpp"
#include "Port.hpp"

namespace midi {

class Reader;

class InternalReader {
public:

	InternalReader() noexcept = default;
	InternalReader(const Port port);
	~InternalReader() noexcept;
	
	static void receive(double timestamp, std::vector<u8>* msg, void* myData);

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
	
private:

	friend class Reader;
	
	std::tuple<std::unique_lock<std::mutex>, InternalReader&> _locked() noexcept;
	std::tuple<std::unique_lock<std::mutex>, const InternalReader&> _locked() const noexcept;

	void _assure() const;

	mutable std::mutex _mutex;
	RtMidiIn _in;
	std::deque<Data> _data;
	std::function<void(InternalReader&)> _callback;
};

}