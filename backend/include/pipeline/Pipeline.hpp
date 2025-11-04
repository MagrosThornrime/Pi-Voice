#pragma once

#include <memory>
#include <vector>
#include <oscillators/Oscillator.hpp>
#include <portaudiocpp/PortAudioCpp.hxx>

class Pipeline: public portaudio::CallbackInterface {
public:
	Pipeline& setSource(const std::shared_ptr<CallbackInterface>& source);
	Pipeline& addLayer(const std::shared_ptr<portaudio::CallbackInterface>& layer);

	int paCallbackFun(const void* inputBuffer, void* outputBuffer, unsigned long numFrames,
		const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags);

private:
	std::vector<std::shared_ptr<portaudio::CallbackInterface>> _layers;
};