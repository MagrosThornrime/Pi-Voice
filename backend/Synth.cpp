#include <portaudiocpp/PortAudioCpp.hxx>
#include <iostream>
#include <thread>
#include <chrono>
#include <oscillators/SawtoothOscillator.hpp>
#include <oscillators/SineOscillator.hpp>
#include <oscillators/SquareOscillator.hpp>
#include <oscillators/TriangleOscillator.hpp>
#include <oscillators/NoiseOscillator.hpp>
#include <Midi.hpp>
#include <fmt/core.h>
#include <pipeline/Pipeline.hpp>
#include <Filters.hpp>

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
#include <cmath>
#include <iostream>

// saves inputBuffer to .wav file
class FileRecorderCallback: public portaudio::CallbackInterface {
public:
	// capacityFrames: how many frames the internal ring can hold (will be rounded up to power-of-two)
	FileRecorderCallback(const std::string& filename, unsigned channels, unsigned sampleRate, size_t capacityFrames = 1 << 18)
		: _filename(filename),
		_channels(channels),
		_sampleRate(sampleRate),
		_capacityFrames(nextPowerOfTwo(std::max<size_t>(capacityFrames, 256))),
		_capacitySamples(_capacityFrames* channels),
		_mask(_capacitySamples - 1),
		_buffer(_capacitySamples) {}

	~FileRecorderCallback() override {
		stop(); // ensure writer stopped and file patched
	}

	// Start background writer thread and open file for writing
	bool start() {
		if (_running.load(std::memory_order_acquire)) return false;
		_f = std::fopen(_filename.c_str(), "wb+");
		if (!_f) {
			std::perror("FileRecorderCallback: open failed");
			return false;
		}
		if (!writeWavHeaderPlaceholder()) {
			std::fclose(_f); _f = nullptr;
			return false;
		}
		_running.store(true, std::memory_order_release);
		_writerThread = std::thread(&FileRecorderCallback::writerThreadFunc, this);
		return true;
	}

	// Stop background writer, finalize header
	void stop() {
		if (!_running.load(std::memory_order_acquire)) {
			if (_f) { std::fclose(_f); _f = nullptr; }
			return;
		}
		_running.store(false, std::memory_order_release);
		_cv.notify_one();
		if (_writerThread.joinable()) _writerThread.join();
		patchWavHeader();
		if (_f) { std::fclose(_f); _f = nullptr; }
	}

	// number of dropped samples (floats) due to ring overflow
	uint64_t droppedSamples() const { return _droppedSamples.load(std::memory_order_relaxed); }

	// -------------------------
	// Real-time safe callback: copies inputBuffer (float*) into ring buffer.
	// If inputBuffer == nullptr, writes zeros for the block.
	// NOTE: this callback should be installed in your PortAudio pipeline as the stream callback
	// (or called from your pipeline). It ignores outputBuffer.
	int paCallbackFun(const void* inputBuffer, void* /*outputBuffer*/, unsigned long numFrames,
		const PaStreamCallbackTimeInfo* /*timeInfo*/, PaStreamCallbackFlags statusFlags) override {
// Optionally track statusFlags (non-blocking update)
		if (statusFlags & paInputOverflow) {
			_statusInputOverflow.fetch_add(1, std::memory_order_relaxed);
		}
		if (statusFlags & paOutputUnderflow) {
			_statusOutputUnderflow.fetch_add(1, std::memory_order_relaxed);
		}

		const float* in = static_cast<const float*>(inputBuffer);
		const bool haveInput = (in != nullptr);

		const size_t samplesToPush = static_cast<size_t>(numFrames) * _channels;
		if (samplesToPush == 0) return paContinue;

		uint64_t w = _writeIndex.load(std::memory_order_relaxed);
		uint64_t r = _readIndex.load(std::memory_order_acquire);
		uint64_t available = w - r;
		uint64_t freeSpace = _capacitySamples - available;

		if (freeSpace == 0) {
			// ring full: drop full block
			_droppedSamples.fetch_add(samplesToPush, std::memory_order_relaxed);
			return paContinue;
		}
		const size_t toWrite = static_cast<size_t>(std::min<uint64_t>(samplesToPush, freeSpace));
		const size_t pos = static_cast<size_t>(w & _mask);

		// write in one or two memcpy calls to handle wrap
		const size_t first = std::min(toWrite, _capacitySamples - pos);
		if (haveInput) {
			std::memcpy(&_buffer[pos], in, first * sizeof(float));
			if (first < toWrite) {
				std::memcpy(&_buffer[0], in + first, (toWrite - first) * sizeof(float));
			}
		} else {
			// inputBuffer == nullptr -> write zeros
			std::memset(&_buffer[pos], 0, first * sizeof(float));
			if (first < toWrite) {
				std::memset(&_buffer[0], 0, (toWrite - first) * sizeof(float));
			}
		}

		_writeIndex.store(w + toWrite, std::memory_order_release);

		// cheap notify for writer thread; allowed in callback
		_cv.notify_one();

		return paContinue;
	}

	// Optional: start recording filename (replace file). Thread-safe only when stopped.
	void setFilename(const std::string& filename) {
		if (_running.load(std::memory_order_acquire)) return;
		_filename = filename;
	}

	// Diagnostics
	uint64_t statusInputOverflowCount() const { return _statusInputOverflow.load(); }
	uint64_t statusOutputUnderflowCount() const { return _statusOutputUnderflow.load(); }
	unsigned channels() const { return _channels; }
	unsigned sampleRate() const { return _sampleRate; }

private:
	// ring helpers
	static size_t nextPowerOfTwo(size_t v) {
		size_t p = 1;
		while (p < v) p <<= 1;
		return p;
	}

	void writerThreadFunc() {
		const size_t maxChunkSamples = 4096 * _channels;
		std::vector<float> tmp;
		tmp.reserve(maxChunkSamples);

		while (_running.load(std::memory_order_acquire) || (_readIndex.load(std::memory_order_relaxed) != _writeIndex.load(std::memory_order_acquire))) {
			uint64_t w = _writeIndex.load(std::memory_order_acquire);
			uint64_t r = _readIndex.load(std::memory_order_relaxed);
			uint64_t avail = w - r;
			if (avail == 0) {
				std::unique_lock<std::mutex> lk(_cvMutex);
				_cv.wait_for(lk, std::chrono::milliseconds(50));
				continue;
			}

			size_t chunk = static_cast<size_t>(std::min<uint64_t>(avail, maxChunkSamples));
			tmp.resize(chunk);

			size_t pos = static_cast<size_t>(r & _mask);
			size_t first = std::min(chunk, _capacitySamples - pos);
			std::memcpy(tmp.data(), &_buffer[pos], first * sizeof(float));
			if (first < chunk) {
				std::memcpy(tmp.data() + first, &_buffer[0], (chunk - first) * sizeof(float));
			}

			size_t written = std::fwrite(tmp.data(), sizeof(float), chunk, _f);
			if (written != chunk) {
				// log and continue; keep read index in sync for data we consumed
				std::perror("FileRecorderCallback: fwrite failed");
			}
			_dataBytesWritten += written * sizeof(float);
			_readIndex.store(r + written, std::memory_order_release);
		}

		std::fflush(_f);
	}

	// WAV header helpers
	void writeLE_u32(FILE* f, uint32_t v) {
		uint8_t buf[4];
		buf[0] = (uint8_t)(v & 0xff);
		buf[1] = (uint8_t)((v >> 8) & 0xff);
		buf[2] = (uint8_t)((v >> 16) & 0xff);
		buf[3] = (uint8_t)((v >> 24) & 0xff);
		std::fwrite(buf, 1, 4, f);
	}
	void writeLE_u16(FILE* f, uint16_t v) {
		uint8_t buf[2];
		buf[0] = (uint8_t)(v & 0xff);
		buf[1] = (uint8_t)((v >> 8) & 0xff);
		std::fwrite(buf, 1, 2, f);
	}

	bool writeWavHeaderPlaceholder() {
		// 32-bit float WAV (audioFormat = 3)
		std::fwrite("RIFF", 1, 4, _f);
		writeLE_u32(_f, 0); // placeholder chunk size
		std::fwrite("WAVE", 1, 4, _f);

		std::fwrite("fmt ", 1, 4, _f);
		writeLE_u32(_f, 16); // fmt chunk size
		writeLE_u16(_f, 3); // format = IEEE float
		writeLE_u16(_f, static_cast<uint16_t>(_channels));
		writeLE_u32(_f, static_cast<uint32_t>(_sampleRate));
		uint32_t byteRate = _sampleRate * _channels * sizeof(float);
		writeLE_u32(_f, byteRate);
		uint16_t blockAlign = static_cast<uint16_t>(_channels * sizeof(float));
		writeLE_u16(_f, blockAlign);
		writeLE_u16(_f, static_cast<uint16_t>(8 * sizeof(float)));

		std::fwrite("data", 1, 4, _f);
		writeLE_u32(_f, 0); // placeholder data size
		return true;
	}

	void patchWavHeader() {
		if (!_f) return;
		uint64_t dataSize = _dataBytesWritten;
		uint64_t riffSize = 36 + dataSize;

		std::fseek(_f, 4, SEEK_SET);
		writeLE_u32(_f, static_cast<uint32_t>(riffSize));
		std::fseek(_f, 40, SEEK_SET);
		writeLE_u32(_f, static_cast<uint32_t>(dataSize));
		std::fflush(_f);
	}

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

int main() {

	try {
		portaudio::AutoSystem autoSys;

		portaudio::System& sys = portaudio::System::instance();

		portaudio::Device& outputDevice = sys.defaultOutputDevice();

		std::cout << "Using output device: " << outputDevice.name() << std::endl;

		portaudio::DirectionSpecificStreamParameters outParams(
			outputDevice,
			2,                      // 2 channels (stereo)
			portaudio::FLOAT32,     // sample format
			true,                  // interleaved
			outputDevice.defaultLowOutputLatency(),
			nullptr
		);

		portaudio::StreamParameters streamParams(
			portaudio::DirectionSpecificStreamParameters::null(), // input
			outParams,
			44100,
			256,
			paClipOff
		);

		auto oscillator = std::make_shared<SineOscillator>(44100.0f);
		oscillator->setFrequency(0);

		auto recorder = std::make_shared<FileRecorderCallback>("capture_in.wav", 2, 44100);
		if (!recorder->start()) {
			std::cerr << "Failed to start recorder\n";
			return 1;
		}


		Pipeline pipeline;
		pipeline
			.setSource(oscillator)
			.addLayer(std::make_shared<LowPassFilter>(2, streamParams.outputParameters().numChannels(), 1000, streamParams.sampleRate()))
			.addLayer(recorder)
			//.addLayer(std::make_shared<LowPassFilter2>(1000, streamParams.sampleRate(), streamParams.outputParameters().numChannels()))
			//.addLayer(std::make_shared<BandPassFilter>(1000, 1, streamParams.sampleRate(), streamParams.outputParameters().numChannels()));
			;
		//oscillator.setAmplitude(0.05f);

		portaudio::CallbackInterface& pipelineInt = pipeline;
		portaudio::InterfaceCallbackStream stream(
			streamParams,
			pipeline
		);

		stream.start();
		std::cout << "Stream started." << std::endl;

		{
			//oscillator->setFrequency(4000);
			auto sineThread = std::jthread([&](std::stop_token stopToken) {
				try {
					for (u32 i = 0; not stopToken.stop_requested(); ++i, std::this_thread::sleep_for(std::chrono::milliseconds(10))) {
						oscillator->setFrequency(200.0f + 3800.0f * (0.5f + 0.5f * std::sin(3.14159265f * i / 100.f)));
					}
				} catch (std::exception& e) {
					fmt::println("{}", e.what());
				}
			});

			(void)getchar();
		}

		recorder->stop();

		/*{
			for (auto&& port : midi::Ports::list()) {
				fmt::println("{}: {}", port.num, port.name);
			}
			fmt::print("> ");
			u32 which{};
			std::cin >> which;
			std::cin.ignore();

			auto midiThread = std::jthread([&](std::stop_token stopToken) {
				try {
					midi::Reader reader;
					reader.open(midi::Ports::getByNum(which));
					reader.readAll();

					for (; not stopToken.stop_requested();) {
						auto data = reader.read();
						if (data.hasVelocity()) {
							auto note = data.note();
							fmt::print("{}{}", note.name, data.status() == midi::Data::noteOn ? 'v' : '^');
							if (data.status() == midi::Data::noteOn) {
								oscillator.setAmplitude(1);
								oscillator.setFrequency(note.freq);
								fmt::println(" {}", data.velocity());
							}
							if (data.status() == midi::Data::noteOff) {
								oscillator.setAmplitude(0);
								fmt::println("");
							}
						}
					}
				} catch (std::exception& e) {
					fmt::println("{}", e.what());
				}
			});

			(void)getchar();
		}*/

		stream.stop();
		stream.close();
		std::cout << "Stream stopped and closed." << std::endl;

	} catch (const portaudio::PaException& e) {
		std::cerr << "PortAudio exception: " << e.paErrorText() << std::endl;
		return 1;
	} catch (const std::exception& e) {
		std::cerr << "Standard exception: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
