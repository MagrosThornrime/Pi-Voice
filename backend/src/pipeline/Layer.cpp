#include <pipeline/Layer.hpp>

namespace pipeline {

Layer::Layer(const u32 channels, const f32 sampleRate) : _channels(channels), _sampleRate(sampleRate) {}

void Layer::refresh() {}

void Layer::processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) {}
}
