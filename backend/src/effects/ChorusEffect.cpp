#include <effects/ChorusEffect.hpp>
#include <cmath>
#include <numbers>

namespace effects {
void ChorusEffect::processSound(std::vector<f32>& inputBuffer,
                               std::vector<f32>& outputBuffer,
                               u32 frames)
{
    u32 idx = 0;

    for (u32 i = 0; i < frames; i++)
    {
        f32 lfo = std::sin(2.0f * std::numbers::pi * _phase);
        _phase += _modFrequency / _sampleRate;
        _phase -= std::floor(_phase);

        f32 delayFrames = (_baseDelayFactor + _modDepth * lfo) * _sampleRate;

        delayFrames = std::clamp(delayFrames, 0.0f, (f32)(_bufferFrames - 2));

        f32 delaySamples = delayFrames * _channels;

        for (u32 ch = 0; ch < _channels; ch++, idx++)
        {
            f32 readPos = (f32)_index - delaySamples + (f32)ch;

            readPos = std::fmod(readPos + _buffer.size(), _buffer.size());

            i32 i0 = readPos;
            i32 i1 = (i0 + _channels) % _buffer.size();
            f32 frac = readPos - i0;

            f32 delayed = _buffer[i0] * (1.0f - frac) + _buffer[i1] * frac;

            f32 in = inputBuffer[idx];
            outputBuffer[idx] = in * (1.0f - _wetAmount) + delayed * _wetAmount;

            _buffer[_index] = in * (1.0f - _feedback) + delayed * _feedback;

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

ChorusEffect::ChorusEffect(const u32 channels, const f32 sampleRate) : Effect(channels, sampleRate) {
    refresh();
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