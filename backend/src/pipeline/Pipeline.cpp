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

	if (i >= _layers.size()) {
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

	_layers.front()->paCallbackFun(inputBuffer, outputBuffer, numFrames, timeInfo, statusFlags);

	for (auto&& layer : _layers | ranges::views::drop(1)) {
		layer->paCallbackFun(outputBuffer, outputBuffer, numFrames, timeInfo, statusFlags);
	}
	return paContinue;
}
}