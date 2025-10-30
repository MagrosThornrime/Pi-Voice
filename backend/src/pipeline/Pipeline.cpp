#include <pipeline/Pipeline.hpp>
#include <range/v3/view/drop.hpp>
#include <fmt/core.h>

using InterfaceRef = std::shared_ptr<portaudio::CallbackInterface>;

Pipeline& Pipeline::setSource(const InterfaceRef& source) {
	_layers.insert(_layers.begin(), source);
	return *this;
}

Pipeline& Pipeline::addLayer(const InterfaceRef& layer) {
	_layers.push_back(layer);
	return *this;
}

int Pipeline::paCallbackFun(const void* inputBuffer, void* outputBuffer, unsigned long numFrames,
	const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags) {

	_layers.front()->paCallbackFun(inputBuffer, outputBuffer, numFrames, timeInfo, statusFlags);

	for (auto&& layer : _layers | ranges::views::drop(1)) {
		layer->paCallbackFun(outputBuffer, outputBuffer, numFrames, timeInfo, statusFlags);
	}
	return paContinue;
}