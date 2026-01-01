#pragma once
#include <utils/SPSCQueue.hpp>
#include <string>
#include <array>
#include <Types.hpp>
#include <vector>
#include <span>

namespace seq {

class MemoryRecorder {
public:
	MemoryRecorder() = default;
	/// @brief Initializes recorder
	MemoryRecorder(const u32 sampleRate, const u32 channels = 2, const f32 seconds = 1);

	/// @brief Saves given data to the buffer
	/// @brief Data could be trimmed if it would not fit inside buffer
	bool write(std::span<const f32> data);

	/// @brief Reinitializes recorder
	void set(const u32 sampleRate, const u32 channels, const f32 seconds);
	/// @brief Starts recorder
	void start();
	/// @brief Stops recorder
	void stop();

	/// @brief Returns recorder sample
	[[nodiscard]] std::vector<f32> getResult();

	bool running() const;

private:
	bool _isRunning = false;
	u32 _channels{}, _sampleRate{};
	f32 _seconds{};
	std::vector<f32> _buffer;

	bool _valid() const;
};

}