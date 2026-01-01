#include <sequencer/MemoryRecorder.hpp>

namespace seq {

bool MemoryRecorder::_valid() const {
	return this->_channels != 0 and this->_sampleRate != 0 and this->_seconds != 0 and this->_buffer.capacity() != 0;
}

MemoryRecorder::MemoryRecorder(const u32 sampleRate, const u32 channels, const f32 seconds) {
	set(sampleRate, channels, seconds);
}

void MemoryRecorder::set(const u32 sampleRate, const u32 channels, const f32 seconds) {
	_channels = channels;
	_sampleRate = sampleRate;
	_seconds = seconds;
	_buffer.clear();
	_buffer.reserve(_channels * _sampleRate * _seconds);
}

bool MemoryRecorder::running() const {
	return _isRunning;
}

bool MemoryRecorder::write(std::span<const f32> data) {
	if (not _isRunning or not _valid()) {
		return false;
	}

	const auto writable = std::min(_buffer.capacity() - _buffer.size(), data.size());
	if (writable == 0) {
		return false;
	}

	_buffer.insert(_buffer.end(), data.begin(), data.begin() + writable);
	return true;
}

void MemoryRecorder::start() {
	if (not _valid()) return;
	_buffer.clear();
	_isRunning = true;
}

void MemoryRecorder::stop() {
	if (not _valid()) return;
	_isRunning = false;
}

std::vector<f32> MemoryRecorder::getResult() {
	if (not _valid()) return {};
	std::vector<f32> result = std::move(_buffer);
	_buffer = {};
	_buffer.reserve(_channels * _sampleRate * _seconds);
	return result;
}

}