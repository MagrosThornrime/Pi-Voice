#include <pipeline/Pipeline.hpp>
#include <range/v3/view/drop.hpp>
#include <fmt/core.h>

namespace pipeline {

using LayerRef = std::shared_ptr<Layer>;

LayerRef Pipeline::add(const LayerRef& layer, u32 i) {
	if (not layer) {
		return layer;
	}

	if (i >= _layers.size() and i != 0) {
		i = _layers.size() - 1;
	}

	std::unique_lock lock(_layersMutex);
	_layers.insert(_layers.begin() + i, layer);
	return layer;
}

LayerRef Pipeline::remove(const u32 i) {
	if (i >= _layers.size()) {
		return nullptr;
	}

	std::unique_lock lock(_layersMutex);
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
	return static_cast<u32>(_layers.size());
}

int Pipeline::paCallbackFun(const void* /*inputBuffer*/, void* outputBuffer,
							unsigned long numFrames,
							const PaStreamCallbackTimeInfo* /*timeInfo*/,
							PaStreamCallbackFlags /*statusFlags*/)
{
	float* out = (float*)outputBuffer;
	const u32 samplesNeeded = numFrames * _channels;

	for (u32 i = 0; i < samplesNeeded; i++) {
		float sample;
		if (!_outputQueue.pop(sample)) {
			sample = 0.0f;
		}

		out[i] = sample;
	}

	_recorder->write(out, numFrames);

	return paContinue;
}

	void Pipeline::_generateSound(std::stop_token stopToken, u32 framesPerCall)
{
	const u32 samplesPerCall = framesPerCall * _channels;
	std::vector<f32> tempBuffer(samplesPerCall);

	while (!stopToken.stop_requested()) {
		// VoiceManager must fill 'samplesPerCall' floats (interleaved channels)
		_voiceManager->generateSound(tempBuffer, framesPerCall); // or change signature to accept samplesPerCall

		// process it using filters/effects; they must expect interleaved samples
		{
			std::shared_lock lock(_layersMutex); // snapshot or lock layers
			for (auto&& layer : _layers) {
				layer->processSound(tempBuffer, tempBuffer, framesPerCall);
			}
		}

		for (float sample : tempBuffer) {
			while (!_outputQueue.push(sample)) // wait until there's space
				std::this_thread::yield();
		}
	}
}


Pipeline::Pipeline(u32 framesPerCall, u32 channels,
				   std::shared_ptr<polyphonic::VoiceManager> voiceManager,
				   std::shared_ptr<fileio::FileRecorder> recorder)
	: _channels(channels),
	  _outputQueue(framesPerCall * channels * 4),
	  _voiceManager(voiceManager),
	  _recorder(recorder)
{
	_producerThread = std::jthread(&Pipeline::_generateSound, this, framesPerCall);
	_running.store(true);
}


Pipeline::~Pipeline() {
	if (_producerThread.joinable()) {
		_producerThread.request_stop(); // asks thread to stop
		_producerThread.join();         // wait for finish
	}
	_running.store(false);
}

} // namespace pipeline
