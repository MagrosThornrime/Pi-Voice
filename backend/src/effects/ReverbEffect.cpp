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
        GET_PARAM(feedback);
        GET_PARAM(wetAmount);
    }

    return result;
}

ReverbEffect::ReverbEffect(const u32 channels, const f32 feedback, const f32 wetAmount){
    _set(channels, feedback, wetAmount);
    refresh();
}

ReverbEffect::ReverbEffect(){
    refresh();
}

void ReverbEffect::_set(const u32 channels, const f32 feedback, const f32 wetAmount){
    _channels = channels;
    _feedback = feedback;
    _wetAmount = wetAmount;

}

void ReverbEffect::refresh(){
    for(u32 i=0; i < _buffers.size(); i++){
        _buffers[i].resize(_bufferSizes[i] * _channels);
    }
}


}