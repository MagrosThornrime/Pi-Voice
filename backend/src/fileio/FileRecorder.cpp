#include <fileio/FileRecorder.hpp>
#include <stdexcept>
#include <fmt/core.h>
#include <vector>
#include <array>
#include <bit>
#include <utility>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>

namespace fileio {

FileRecorder::FileRecorder(const u32 sampleRate, const u32 channels, const float seconds, const bool skip):
	_channels{channels},
	_sampleRate{sampleRate},
	_queue{u32(channels * sampleRate * seconds)},
	_skip{skip} {}

int FileRecorder::paCallbackFun(const void* inputBuffer, void* outputBuffer, unsigned long numFrames,
	const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags) {
	auto lock = std::lock_guard(_mutex);

	if(_isRunning) {
		const float* in = (const float*)inputBuffer;
		float* out = (float*)outputBuffer;

		const u32 size = numFrames * _channels;
		std::copy_n(in, size, out); // recorder is a part of a bigger pipeline, it is invisible
		if (not _skip) for (u32 i = 0; i != size; ++i) {
			// force push
			while (!_queue.push(in[i]));
		} else _queue.push(in, size);
	}

	return paContinue;
}

std::string FileRecorder::_getFilename(){
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%d-%m-%Y %H-%M-%S.wav");
	return ss.str();
}

void FileRecorder::start() {
	auto lock = std::lock_guard(_mutex);
	if(_isRunning) {
		return;
	}
	SF_INFO sfInfo{
		.samplerate = (int)_sampleRate,
		.channels = (int)_channels,
		.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT,
		.seekable = true
	};
	std::string filename = _getFilename();
	auto file = sf_open(filename.c_str(), SFM_WRITE, &sfInfo);
	if (!file) {
		std::string err = sf_strerror(nullptr);
		throw std::runtime_error(fmt::format(
			"Failed to open file '{}' for writing: {}",
			filename, err
		));
	}

	_thread = std::jthread(
		&threadFn,
		std::unique_ptr<SNDFILE, void(*)(SNDFILE*)>(
			std::exchange(file, nullptr),
			[](SNDFILE* file) { sf_close(file); }
		),
		std::ref(_queue), _channels, _sampleRate
	);
	_isRunning = true;
}

void FileRecorder::stop() {
	auto lock = std::lock_guard(_mutex);
	if(!_isRunning) {
		return;
	}
	_thread.request_stop();
	_thread.join();
	_isRunning = false;
}

void FileRecorder::threadFn(std::stop_token stopToken, std::unique_ptr<SNDFILE, void(*)(SNDFILE*)> file, utils::SPSCQueue<float>& queue, const u32 channels, const u32 sampleRate) {
	auto buffer = std::vector<float>();
	buffer.reserve(channels * sampleRate);

	auto writeBuffer = [&file, &buffer] {
		sf_write_float(file.get(), buffer.data(), buffer.size());
		buffer.clear();
	};

	while (not stopToken.stop_requested()) {
		float value;
		if (queue.pop(value)) {
			buffer.push_back(value);
		}

		if (buffer.size() == buffer.capacity()) {
			writeBuffer();
		}
	}

	writeBuffer();
}

}