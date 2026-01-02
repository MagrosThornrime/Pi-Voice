#pragma once
#include <pipeline/Layer.hpp>
#include <utils/SPSCQueue.hpp>
#include <string>
#include <array>
#include <Types.hpp>
#include <sndfile.h>
#include <thread>

namespace fileio {
class FileRecorder {
public:
	FileRecorder(const u32 sampleRate, const u32 channels = 2, const float seconds = 1, const bool skip = true);

	void write(const float* buffer, u32 numFrames);

	void setOutputDirectory(const std::string& dir);

	void start();
	void stop();
private:
	std::string _getFilename();
	bool _canWriteToDirectory(const std::string& dir);


	bool _isRunning = false;
	std::string _outputDirectory;

	u32 _channels{}, _sampleRate{};
	utils::SPSCQueue<float> _queue;
	std::jthread _thread;
	bool _skip;

	static void _threadFn(std::stop_token stopToken, std::unique_ptr<SNDFILE, void(*)(SNDFILE*)> file, utils::SPSCQueue<float>& queue, const u32 channels, const u32 sampleRate);
};
}