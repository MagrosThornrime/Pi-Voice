#include <pipeline/Pipeline.hpp>
#include <fmt/core.h>
#include <algorithm>
#include <functional>
#include <thread>

namespace pipeline {

using LayerRef = std::shared_ptr<Layer>;

Pipeline::Pipeline(u32 framesPerCall, u32 channels, f32 sampleRate,
	std::shared_ptr<polyphonic::VoiceManager> voiceManager,
	std::shared_ptr<fileio::FileRecorder> recorder,
	std::shared_ptr<seq::Sequencer> sequencer)
	: _channels(channels),
	_sampleRate(sampleRate),
	_outputQueue(framesPerCall * channels * 4),
	_voiceManager(voiceManager),
	_recorder(recorder),
	_sequencer(sequencer) {
	_producerThread = std::jthread([this, framesPerCall](std::stop_token stopToken) {
		_generateSound(stopToken, framesPerCall);
	});
}

LayerRef Pipeline::add(const LayerRef& layer, std::optional<u32> index) {
	if (!layer) {
		return nullptr;
	}

	std::lock_guard lock(_layersMutex);

	const u32 size = _layers.size();

	u32 i = index.value_or(size);
	i = std::min(i, size);

	_layers.insert(_layers.begin() + i, layer);
	return layer;
}

LayerRef Pipeline::remove(const u32 i) {
	auto lock = std::lock_guard(_layersMutex);
	if (i >= _layers.size()) {
		return nullptr;
	}

	LayerRef result = std::move(_layers[i]);
	_layers.erase(_layers.begin() + i);
	return result;
}

void Pipeline::move(const u32 curr, const u32 target) {
	auto lock = std::lock_guard(_layersMutex);
	const u32 size = _layers.size();
	if (curr >= size || curr == target){
		return;
	}

	LayerRef item = std::move(_layers[curr]);
	_layers.erase(_layers.begin() + curr);

	const u32 newSize = _layers.size();
	u32 insertIndex = target;
	if(curr < insertIndex){
		insertIndex--;
	}
	insertIndex = std::min(insertIndex, newSize);
	_layers.insert(_layers.begin() + insertIndex, std::move(item));
}

void Pipeline::swap(const u32 i1, const u32 i2) {
	auto lock = std::lock_guard(_layersMutex);
	if (i1 >= _layers.size() || i2 >= _layers.size()){
		return;
	}
	std::swap(_layers[i1], _layers[i2]);
}

LayerRef Pipeline::get(const u32 i) const {
	auto lock = std::lock_guard(_layersMutex);
	if (i >= _layers.size()){
		return nullptr;
	}
	return _layers[i];
}

u32 Pipeline::length() const {
	auto lock = std::lock_guard(_layersMutex);
	return _layers.size();
}

int Pipeline::paCallbackFun(const void* /*inputBuffer*/, void* outputBuffer, unsigned long numFrames,
	const PaStreamCallbackTimeInfo* /*timeInfo*/, PaStreamCallbackFlags /*statusFlags*/) {

	float* out = static_cast<float*>(outputBuffer);
	const u32 samplesNeeded = static_cast<u32>(numFrames) * _channels;

	for (u32 i = 0; i < samplesNeeded; ++i) {
		float sample;
		if (!_outputQueue.pop(sample)) {
			sample = 0.0f;
		}
		out[i] = sample;
	}

	if (_recorder) {
		_recorder->write(out, numFrames);
	}

	return paContinue;
}

void Pipeline::_mixWithSequencer(std::vector<f32>& buffer) {
	if (_sequencer->isActive()) {
		auto iter = _sequencer->iter();
		for (auto&& v : buffer) {
			v += *iter;
			++iter;
		}
	}
	for (auto&& v : buffer) {
		v /= 2;
	}
}

void Pipeline::_generateSound(std::stop_token stopToken, u32 framesPerCall) {
	const u32 samplesPerCall = framesPerCall * _channels;
	std::vector<float> tempBuffer(samplesPerCall);

	while (!stopToken.stop_requested()) {
		_voiceManager->generateSound(tempBuffer, framesPerCall);
		{
			auto lock = std::lock_guard(_layersMutex);
			for (auto&& layer : _layers) {
				layer->processSound(tempBuffer, tempBuffer, framesPerCall);
			}
		}

		_sequencer->writeToRecorder(tempBuffer);
		_mixWithSequencer(tempBuffer);

		for (u32 i = 0; i < samplesPerCall; ++i) {
			while (!_outputQueue.push(tempBuffer[i])) {
				std::this_thread::yield();
			}
		}
	}
}

u32 Pipeline::getChannels() const {
	return _channels;
}

f32 Pipeline::getSampleRate() const {
	return _sampleRate;
}

void Pipeline::setAmplitude(const f32 value) {

}

} // namespace pipeline
