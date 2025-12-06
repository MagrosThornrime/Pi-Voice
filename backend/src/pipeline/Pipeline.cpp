#include <pipeline/Pipeline.hpp>
#include <range/v3/view/drop.hpp>
#include <fmt/core.h>

namespace pipeline {

using LayerRef = std::shared_ptr<Layer>;

//Pipeline& Pipeline::setSource(const InterfaceRef& source) {
//	_layers.insert(_layers.begin(), source);
//	return *this;
//}
//
//Pipeline& Pipeline::addLayer(const InterfaceRef& layer) {
//	_layers.push_back(layer);
//	return *this;
//}

LayerRef Pipeline::add(const LayerRef& layer, u32 i) {
	if (not layer) {
		return layer;
	}

	if (i >= _layers.size() and i != 0) {
		i = _layers.size() - 1;
	}

	_layers.insert(_layers.begin() + i, layer);
	return layer;
}

LayerRef Pipeline::remove(const u32 i) {
	if (i >= _layers.size()) {
		return nullptr;
	}

	LayerRef result = std::move(_layers[i]);
	_layers.erase(_layers.begin() + i);
	return result;
}

void Pipeline::move(const u32 curr, const u32 target) {
	add(remove(curr), target);
}

void Pipeline::swap(const u32 i1, const u32 i2) {
	if (i1 >= _layers.size() or i2 >= _layers.size()) {
		return;
	}

	std::swap(_layers[i1], _layers[i2]);
}

LayerRef Pipeline::get(const u32 i) const {
	if (i >= _layers.size()) {
		return nullptr;
	}

	return _layers[i];
}

u32 Pipeline::length() const {
	return (u32)_layers.size();
}

int Pipeline::paCallbackFun(const void* inputBuffer, void* outputBuffer, unsigned long numFrames,
	const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags) {

	// get data from the output queue if available (wait if not)
	for (i32 i = 0; i < numFrames; i++) {
		sound = _outputQueue.pop();
		outputBuffer[i] = sound[i];
	}
	// push data to the recorder
	_recorder->paCallbackFun(outputBuffer, outputBuffer, numFrames, timeInfo, statusFlags);
	return paContinue;
}

void _generateSound(std::stop_token stopToken, u32 framesPerCall){
	// avoid calling the functions too often by using a buffer
	std::vector<f32> tempBuffer(framesPerCall);
	while(!stop_token.stop_requested()) {
		// generate the starting sound
		_voiceManager->generateSound(tempBuffer, framesPerCall);
		// process it using filters/effects
		for (auto&& layer : _layers) {
			layer->processSound(tempBuffer, tempBuffer, framesPerCall);
		}
		// return the sound
		for (auto&& sound : tempBuffer) {
			_outputQueue.put(sound);
		}
	}

}

Pipeline::Pipeline() {
	// initialize the thread
}
}