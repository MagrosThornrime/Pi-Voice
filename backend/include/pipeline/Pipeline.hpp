#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <oscillators/Oscillator.hpp>
#include <Portaudio.hpp>
#include "Layer.hpp"
#include <utils/SPSCQueue.hpp>

namespace pipeline {
class Pipeline: public pa::CallbackInterface {
public:
	std::shared_ptr<Layer> add(const std::shared_ptr<Layer>& layer, u32 i = (u32)-1);
	std::shared_ptr<Layer> remove(const u32 i);
	void move(const u32 curr, const u32 target);
	void swap(const u32 i1, const u32 i2);
	std::shared_ptr<Layer> get(const u32 i) const;
	u32 length() const;

	int paCallbackFun(const void* inputBuffer, void* outputBuffer, unsigned long numFrames,
		const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags);

	Pipeline();

private:
	std::vector<std::shared_ptr<Layer>> _layers;

	std::shared_ptr<std::jthread> _producerThread;
	utils::SPSCQueue<f32> _outputQueue;

	void _generateSound(const void* inputBuffer, void* outputBuffer, unsigned long numFrames,
		const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags);

};
}