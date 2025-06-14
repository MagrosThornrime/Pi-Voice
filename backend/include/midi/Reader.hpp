#pragma once

#include "InternalReader.hpp"
#include <mutex>
#include <tuple>

namespace midi {

/// @brief Midi reader class
class Reader {
public:

	/// @brief Default constructor, need open() to be usable
	Reader() noexcept = default;
	/// @brief Port constructor, opens given port
	/// @param port port to open
	Reader(const Port port);

	/// @brief Opens given midi port, closing previously opened
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
	/// @param callback non-returning callable with InternalReader reference as only parameter
	void setCallback(std::function<void(InternalReader&)> callback) noexcept;
	/// @brief Resets callback
	void resetCallback() noexcept;

	/// @brief Checks if port was opened
	bool opened() const noexcept;
	/// @brief Checks if port was opened
	operator bool() const noexcept;
	
	/// @brief Returns tuple with lock of InternalReader and reference to InternalReader
	std::tuple<std::unique_lock<std::mutex>, InternalReader&> locked() noexcept;
	/// @brief Returns tuple with lock of InternalReader and const reference to InternalReader
	std::tuple<std::unique_lock<std::mutex>, const InternalReader&> locked() const noexcept;

private:

	midi::InternalReader _reader;
};

}