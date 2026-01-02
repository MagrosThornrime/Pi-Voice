#include <effects/ReverbEffect.hpp>
#include <fmt/core.h>

namespace effects {
void ReverbEffect::processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames){
    for(u32 i = 0; i < frames * _channels; i++){
        f32 output = 0.0f;
        for(u32 j = 0; j < _buffers.size(); j++){
            u32 delayIndex = _indices[j];
            f32 delayedSample = _buffers[j][delayIndex];
            output += delayedSample;
            _buffers[j][delayIndex] = inputBuffer[i] + delayedSample * _feedback;
            _indices[j] = (delayIndex + 1) % _buffers[j].size();
        }
        outputBuffer[i] = inputBuffer[i] * (1 - _wetAmount) + output / _buffers.size() * _wetAmount;
    }
}

#define SET_PARAM(name) case ReverbParams::name: if(value.type() == typeid(_##name)) { _##name = std::any_cast<decltype(_##name)>(std::move(value)); } break

pipeline::Layer& ReverbEffect::setParam(const u32 param, std::any value){
    switch (param) {
		SET_PARAM(bufferFrames);
        SET_PARAM(feedback);
        SET_PARAM(wetAmount);
    }
    refresh();
    return *this;
}

#define GET_PARAM(name) case ReverbParams::name: result = _##name; break

std::any ReverbEffect::getParam(const u32 param){
    std::any result;

    switch (param) {
		GET_PARAM(bufferFrames);
        GET_PARAM(feedback);
        GET_PARAM(wetAmount);
    }

    return result;
}

ReverbEffect::ReverbEffect(const u32 channels, const f32 sampleRate) : Effect(channels, sampleRate) {
    refresh();
}

void ReverbEffect::_set(const u32 bufferFrames, const f32 feedback, const f32 wetAmount){
	_bufferFrames = bufferFrames;
    _feedback = feedback;
    _wetAmount = wetAmount;
}

void ReverbEffect::refresh(){
	u32 firstSize = _bufferFramesFactors[0] * _bufferFrames * _channels;
	if(firstSize == _buffers[0].size()){
        return;
    }
	for(u32 i = 0; i < _buffers.size(); i++){
		std::vector<f32> newBuffer(_bufferFramesFactors[i] * _bufferFrames * _channels);
		auto& currentBuffer = _buffers[i];
		auto& currentIndex = _indices[i];
		if(!_buffers[i].empty()){
			for(u32 j = 0; j < newBuffer.size(); j++){
				newBuffer[i] = currentBuffer[currentIndex];
				currentIndex = (currentIndex + 1) % _buffers[i].size();
			}
		}
		currentBuffer = newBuffer;
		currentIndex = 0;
	}
}

EffectType::Value ReverbEffect::getEffectType(){
    return EffectType::reverb;
}

}