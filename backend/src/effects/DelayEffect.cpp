#include <effects/DelayEffect.hpp>

namespace effects {
void DelayEffect::processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames){
    for(u32 i = 0; i < frames * _channels; i++){
        f32 delayedSample = _delayBuffer[_delayIndex];
        outputBuffer[i] = inputBuffer[i] * (1 - _wetAmount) + delayedSample * _wetAmount;
        _delayBuffer[_delayIndex] = inputBuffer[i] + delayedSample * _feedback;
        _delayIndex = (_delayIndex + 1) % _delayBuffer.size();
    }
}


pipeline::Layer& DelayEffect::setParam(const u32 param, std::any value){
	return *this;
}

std::any DelayEffect::getParam(const u32 param){
	return std::any();
}

DelayEffect::DelayEffect(const u32 channels, const u32 delayTime, const f32 feedback, const f32 wetAmount){
	_set(channels, delayTime, feedback, wetAmount);
	refresh();
}

void DelayEffect::_set(const u32 channels, const u32 delayTime, const f32 feedback, const f32 wetAmount){
	_channels = channels;
	_delayTime = delayTime;
	_feedback = feedback;
	_wetAmount = wetAmount;

}

void DelayEffect::refresh(){
    if(_delayTime * _channels == _delayBuffer.size()){
        return;
    }
	std::vector<f32> newBuffer(_delayTime * _channels);
	if(!_delayBuffer.empty()){
		for(u32 i = 0; i < _delayTime * _channels; i++){
			newBuffer[i] = _delayBuffer[_delayIndex];
			_delayIndex = (_delayIndex + 1) % _delayBuffer.size();
		}
	}
	_delayBuffer = newBuffer;
	_delayIndex = 0;
}


}