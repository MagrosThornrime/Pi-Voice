#include <effects/ModulatorEffect.hpp>
#include <numbers>
#include <cmath>

namespace effects {
    void ModulatorEffect::processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames){
        u32 idx = 0;
        for(u32 i = 0; i < frames; i++){
            f32 modulator = std::sin(_phase);
            for(u32 j = 0; j < _channels; j++, idx++){
                outputBuffer[idx] = modulator * inputBuffer[idx];
            }
            _phase = std::fmod(_phase + _phaseIncrement, 2 * std::numbers::pi);
        }
    }


    pipeline::Layer& ModulatorEffect::setParam(const u32 param, std::any value){
        return *this;
    }

    std::any ModulatorEffect::getParam(const u32 param){
        return std::any();
    }

    ModulatorEffect::ModulatorEffect(const u32 channels, const f32 modFrequency){
        _set(channels, modFrequency);
        refresh();
    }

    void ModulatorEffect::_set(const u32 channels, const f32 modFrequency){
        _channels = channels;
        _modFrequency = modFrequency;
    }

    void ModulatorEffect::refresh(){
        _phaseIncrement = 2.0f * std::numbers::pi * _modFrequency / _sampleRate;
    }


}