#include <fileio/FileRecorder.hpp>
#include <stdexcept>
#include <fmt/core.h>
#include <vector>
#include <array>
#include <bit>
#include <utility>

namespace fileio {

FileRecorder::FileRecorder(const std::string& filename, const u32 sampleRate, const u32 channels, const float seconds, const bool skip):
	_channels{channels},
	_sampleRate{sampleRate},
	_filename{filename},
	_queue{u32(channels * sampleRate * seconds)},
	_skip{skip} {}

int FileRecorder::paCallbackFun(const void* inputBuffer, void* outputBuffer, unsigned long numFrames,
	const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags) {
	const float* in = (const float*)inputBuffer;
	float* out = (float*)outputBuffer;

	const u32 size = numFrames * _channels;
	std::copy_n(in, size, out); // recorder is a part of a bigger pipeline, it is invisible
	if (not _skip) for (u32 i = 0; i != size; ++i) {
		// force push
		while (!_queue.push(in[i]));
	} else _queue.push(in, size);

	return paContinue;
}

void FileRecorder::start() {
	SF_INFO sfInfo{
		.samplerate = (int)_sampleRate,
		.channels = (int)_channels,
		.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT,
		.seekable = true
	};
	auto file = sf_open(_filename.c_str(), SFM_WRITE, &sfInfo);
	if (not file) {
		throw std::runtime_error(fmt::format("Failed to open file '{}' for writing", _filename));
	}

	_thread = std::jthread(
		&threadFn,
		std::unique_ptr<SNDFILE, void(*)(SNDFILE*)>(
			std::exchange(file, nullptr),
			[](SNDFILE* file) { sf_close(file); }
		),
		std::ref(_queue), _channels, _sampleRate
	);
}

void FileRecorder::stop() {
	_thread.request_stop();
	_thread.join();
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