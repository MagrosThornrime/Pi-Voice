#pragma once
#include <portaudiocpp/PortAudioCpp.hxx>
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <algorithm>
#include <chrono>

namespace fileio{
class FileRecorder: public portaudio::CallbackInterface {
public:
	// capacityFrames: how many frames the internal ring can hold (will be rounded up to power-of-two)
	FileRecorder(const std::string& filename, unsigned channels, unsigned sampleRate,
		size_t capacityFrames = 1 << 18);

	~FileRecorder();

	// Start background writer thread and open file for writing
	bool start();

	// Stop background writer, finalize header
	void stop();

	// number of dropped samples (floats) due to ring overflow
	uint64_t droppedSamples() const;

	// -------------------------
	// Real-time safe callback: copies inputBuffer (float*) into ring buffer.
	// If inputBuffer == nullptr, writes zeros for the block.
	// NOTE: this callback should be installed in your PortAudio pipeline as the stream callback
	// (or called from your pipeline). It ignores outputBuffer.
	int paCallbackFun(const void* inputBuffer, void* /*outputBuffer*/, unsigned long numFrames,
		const PaStreamCallbackTimeInfo* /*timeInfo*/, PaStreamCallbackFlags statusFlags) override;

	// Optional: start recording filename (replace file). Thread-safe only when stopped.
	void setFilename(const std::string& filename);

	// Diagnostics
	uint64_t statusInputOverflowCount() const;
	uint64_t statusOutputUnderflowCount() const;
	unsigned channels() const;
	unsigned sampleRate() const;

private:
	// ring helpers
	static size_t nextPowerOfTwo(size_t v);

	void writerThreadFunc();

	// WAV header helpers
	void writeLE_u32(FILE* f, uint32_t v);

	void writeLE_u16(FILE* f, uint16_t v);

	bool writeWavHeaderPlaceholder();

	void patchWavHeader();

	// members
	std::string _filename;
	const unsigned _channels;
	const unsigned _sampleRate;

	const size_t _capacityFrames;
	const size_t _capacitySamples;
	const size_t _mask;
	std::vector<float> _buffer;

	std::atomic<uint64_t> _writeIndex{0};
	std::atomic<uint64_t> _readIndex{0};

	std::atomic<bool> _running{false};
	std::thread _writerThread;
	std::condition_variable _cv;
	std::mutex _cvMutex;

	FILE* _f{nullptr};
	std::atomic<uint64_t> _droppedSamples{0};
	std::atomic<uint64_t> _statusInputOverflow{0};
	std::atomic<uint64_t> _statusOutputUnderflow{0};
	uint64_t _dataBytesWritten{0};
};
}