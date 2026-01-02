#include <effects/ChorusEffect.hpp>
#include <cmath>
#include <numbers>

namespace effects {
void ChorusEffect::processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames){
    u32 idx = 0;
    for(u32 i = 0; i < frames; i++){
        f32 modDelayFactor = _baseDelayFactor + _modDepth * std::sin(2.0 * std::numbers::pi * _phase);
        _phase += _modFrequency / _sampleRate;
        _phase = std::fmod(_phase, 1.0f);
        f32 delayedFrames = modDelayFactor * _sampleRate;

        f32 readFrame = _index / _channels - delayedFrames;
        readFrame = std::fmod(readFrame + _bufferFrames, _bufferFrames);

        i32 frame0 = readFrame;
        i32 frame1 = (frame0 + 1) % _buffer.size();
        f32 factor = readFrame - frame0;

        for(u32 j = 0; j < _channels; j++, idx++){
            u32 channel0 = frame0 + j;
            u32 channel1 = frame1 + j;
            f32 delayedSample = _buffer[channel0] * (1.0f - factor) + _buffer[channel1] * factor;
            outputBuffer[idx] = inputBuffer[idx] * (1.0f - _wetAmount) + delayedSample * _wetAmount;
            _buffer[_index] = inputBuffer[idx] * (1 - _feedback) + delayedSample * _feedback;
            _index = (_index + 1) % _buffer.size();
        }
    }
}

#define SET_PARAM(name) case ChorusParams::name: if(value.type() == typeid(_##name)) { _##name = std::any_cast<decltype(_##name)>(std::move(value)); } break

pipeline::Layer& ChorusEffect::setParam(const u32 param, std::any value){
    switch (param) {
        SET_PARAM(baseDelayFactor);
        SET_PARAM(modFrequency);
        SET_PARAM(modDepth);
        SET_PARAM(bufferFrames);
        SET_PARAM(feedback);
        SET_PARAM(wetAmount);
    }
    refresh();
    return *this;
}

#define GET_PARAM(name) case ChorusParams::name: result = _##name; break

std::any ChorusEffect::getParam(const u32 param){
    std::any result;

    switch (param) {
        GET_PARAM(baseDelayFactor);
        GET_PARAM(modFrequency);
        GET_PARAM(modDepth);
        GET_PARAM(bufferFrames);
        GET_PARAM(feedback);
        GET_PARAM(wetAmount);
    }

    return result;
}

ChorusEffect::ChorusEffect(const u32 channels, const u32 bufferFrames, const f32 feedback, const f32 wetAmount,
        const f32 baseDelayFactor, const f32 modFrequency, const f32 modDepth){
    _set(channels, bufferFrames, feedback, wetAmount, baseDelayFactor, modFrequency, modDepth);
    refresh();
}

ChorusEffect::ChorusEffect(){
    refresh();
}

void ChorusEffect::_set(const u32 channels, const u32 bufferFrames, const f32 feedback, const f32 wetAmount,
        const f32 baseDelayFactor, const f32 modFrequency, const f32 modDepth){
    _channels = channels;
    _bufferFrames = bufferFrames;
    _feedback = feedback;
    _wetAmount = wetAmount;
    _baseDelayFactor = baseDelayFactor;
    _modFrequency = modFrequency;
    _modDepth = modDepth;
}

void ChorusEffect::refresh(){
    _phase = 0.0f;
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

EffectType::Value ChorusEffect::getEffectType(){
    return EffectType::chorus;
}

}