#pragma once
#include <portaudiocpp/PortAudioCpp.hxx>
#include <utils/SPSCQueue.hpp>
#include <string>
#include <array>
#include <Types.hpp>
#include <sndfile.h>
#include <thread>

namespace fileio {
	class FileRecorder: public portaudio::CallbackInterface {
	public:
		FileRecorder(const u32 sampleRate, const u32 channels = 2, const float seconds = 1, const bool skip = true);

		int paCallbackFun(const void* inputBuffer, void* outputBuffer, unsigned long numFrames,
			const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags) override;

		void start();
		void stop();

	private:
		std::string _getFilename();

		u32 _channels{}, _sampleRate{};
		utils::SPSCQueue<float> _queue;
		std::jthread _thread;
		bool _skip;

		static void threadFn(std::stop_token stopToken, std::unique_ptr<SNDFILE, void(*)(SNDFILE*)> file, utils::SPSCQueue<float>& queue, const u32 channels, const u32 sampleRate);
	};
}