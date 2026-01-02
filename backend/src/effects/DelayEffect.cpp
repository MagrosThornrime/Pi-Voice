#include <effects/DelayEffect.hpp>

namespace effects {
void DelayEffect::processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames){
    for(u32 i = 0; i < frames * _channels; i++){
        f32 delayedSample = _buffer[_index];
        outputBuffer[i] = inputBuffer[i] * (1 - _wetAmount) + delayedSample * _wetAmount;
        _buffer[_index] = inputBuffer[i] * (1 - _feedback) + delayedSample * _feedback;
        _index = (_index + 1) % _buffer.size();
    }
}

#define SET_PARAM(name) case DelayParams::name: if(value.type() == typeid(_##name)) { _##name = std::any_cast<decltype(_##name)>(std::move(value)); } break

pipeline::Layer& DelayEffect::setParam(const u32 param, std::any value){
	switch (param) {
		SET_PARAM(bufferFrames);
		SET_PARAM(feedback);
		SET_PARAM(wetAmount);
	}
	refresh();
	return *this;
}

#define GET_PARAM(name) case DelayParams::name: result = _##name; break

std::any DelayEffect::getParam(const u32 param){
	std::any result;

	switch (param) {
		GET_PARAM(bufferFrames);
		GET_PARAM(feedback);
		GET_PARAM(wetAmount);
	}

	return result;
}

DelayEffect::DelayEffect(const u32 channels, const f32 sampleRate) : Effect(channels, sampleRate) {
    refresh();
}

void DelayEffect::_set(const u32 bufferFrames, const f32 feedback, const f32 wetAmount){
	_bufferFrames = bufferFrames;
	_feedback = feedback;
	_wetAmount = wetAmount;
}

void DelayEffect::refresh(){
    if(_bufferFrames * _channels == _buffer.size()){
        return;
    }
	std::vector<f32> newBuffer(_bufferFrames * _channels);
	if(!_buffer.empty()){
		for(u32 i = 0; i < _bufferFrames * _channels; i++){
			newBuffer[i] = _buffer[_index];
			_index = (_index + 1) % _buffer.size();
		}
	}
	_buffer = newBuffer;
	_index = 0;
}

EffectType::Value DelayEffect::getEffectType(){
    return EffectType::delay;
}

}