#include <effects/ReverbEffect.hpp>
#include <fmt/core.h>

namespace effects {
    void ReverbEffect::processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames){
        for(u32 i = 0; i < frames * _channels; i++){
            f32 output = 0.0f;
            for(u32 j = 0; j < _delayBuffers.size(); j++){
                u32 delayIndex = _delayIndices[j];
                f32 delayedSample = _delayBuffers[j][delayIndex];
                output += delayedSample;
                _delayBuffers[j][delayIndex] = inputBuffer[i] + delayedSample * _feedback;
                _delayIndices[j] = (delayIndex + 1) % _delayBuffers[j].size();
            }
            outputBuffer[i] = inputBuffer[i] * (1 - _wetAmount) + output / _delayBuffers.size() * _wetAmount;
        }
    }


    pipeline::Layer& ReverbEffect::setParam(const u32 param, std::any value){
        return *this;
    }

    std::any ReverbEffect::getParam(const u32 param){
        return std::any();
    }

    ReverbEffect::ReverbEffect(const u32 channels, const f32 feedback, const f32 wetAmount){
        _set(channels, feedback, wetAmount);
        for(u32 i=0; i < _delayBuffers.size(); i++){
            _delayBuffers[i].resize(_bufferSizes[i]);
        }
    }

    void ReverbEffect::_set(const u32 channels, const f32 feedback, const f32 wetAmount){
        _channels = channels;
        _feedback = feedback;
        _wetAmount = wetAmount;

    }

    void ReverbEffect::refresh(){}


}