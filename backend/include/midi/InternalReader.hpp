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

/// @brief Same as Reader, but any operation assumes you aquired lock on it
class InternalReader {
public:

	/// @brief Destructor, closes internal midi handler.
	~InternalReader() noexcept;
	
	/// @brief Midi event handler
	/// @param timestamp time since last event, in seconds
	/// @param msg vector with received data
	/// @param myData pointer to InternalReader (this)
	static void receive(double timestamp, std::vector<u8>* msg, void* myData);

	/// @brief Opens given midi port, closing previously opened
	/// @param port midi port to open
	void open(const Port port);
	/// @brief Closes opened midi port, if any
	void close() noexcept;

	/// @brief Reads one event
	Data read();
	/// @brief Reads given number of events
	std::vector<Data> readSome(const u32 n);
	/// @brief Reads all events
	std::vector<Data> readAll();
	/// @brief Returns queued events count
	u32 queueSize() const;
	/// @brief Clears event queue
	void clear() noexcept;

	/// @brief Sets event callback
	/// @details Callback has lock on reader while executing
	/// @param callback non-returning callable with InternalReader reference as only parameter
	void setCallback(std::function<void(InternalReader&)> callback) noexcept;
	/// @brief Resets callback
	void resetCallback() noexcept;

	/// @brief Checks if port was opened
	bool opened() const noexcept;
	/// @brief Checks if port was opened
	operator bool() const noexcept;
	
private:

	// expects to be opened
	InternalReader() noexcept = default;
	// opens port
	InternalReader(const Port port);

	friend class Reader;
	
	// returns tuple with lock to InternalReader and its reference
	std::tuple<std::unique_lock<std::mutex>, InternalReader&> _locked() noexcept;
	std::tuple<std::unique_lock<std::mutex>, const InternalReader&> _locked() const noexcept;

	// throws if preconditions are not met
	void _assure() const;

	mutable std::mutex _mutex;
	RtMidiIn _in;
	std::deque<Data> _data;
	std::function<void(InternalReader&)> _callback;
};

}