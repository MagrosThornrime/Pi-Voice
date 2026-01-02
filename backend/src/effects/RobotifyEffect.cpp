#include <effects/RobotifyEffect.hpp>
#include <numbers>
#include <cmath>

namespace effects {
void RobotifyEffect::processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames){
    u32 idx = 0;
    for(u32 i = 0; i < frames; i++){
        f32 modulator = std::sin(_phase);
        for(u32 j = 0; j < _channels; j++, idx++){
            outputBuffer[idx] = modulator * inputBuffer[idx];
        }
        _phase += _phaseIncrement;
        _phase = std::fmod(_phase, 2 * std::numbers::pi);
    }
}

#define SET_PARAM(name) case RobotifyParams::name: if(value.type() == typeid(_##name)) { _##name = std::any_cast<decltype(_##name)>(std::move(value)); } break

pipeline::Layer& RobotifyEffect::setParam(const u32 param, std::any value){
    switch (param) {
        SET_PARAM(modFrequency);
    }
    refresh();
    return *this;
}

#define GET_PARAM(name) case RobotifyParams::name: result = _##name; break

std::any RobotifyEffect::getParam(const u32 param){
    std::any result;

    switch (param) {
        GET_PARAM(modFrequency);
    }

    return result;
}

RobotifyEffect::RobotifyEffect(const u32 channels, const f32 sampleRate) : Effect(channels, sampleRate) {
    refresh();
}

void RobotifyEffect::refresh(){
    _phase = 0.0f;
    _phaseIncrement = 2.0f * std::numbers::pi * _modFrequency / _sampleRate;
}

EffectType::Value RobotifyEffect::getEffectType(){
    return EffectType::robotify;
}

}