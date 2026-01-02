#pragma once

#include <Portaudio.hpp>
#include <any>
#include <Types.hpp>
#include <vector>


namespace pipeline {

class Layer {
public:
	Layer(const u32 channels, const f32 sampleRate);

	virtual Layer& setParam(const u32 param, std::any value) = 0;
	virtual std::any getParam(const u32 param) = 0;

	virtual void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames);
	virtual void refresh();

	virtual ~Layer() = default;
protected:
	u32 _channels;
	f32 _sampleRate;
};

}