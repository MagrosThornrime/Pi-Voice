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

#define SET_PARAM(name) case FilterParams::name: if(value.type() == typeid(_##name)) { _##name = std::any_cast<decltype(_##name)>(std::move(value)); } break

pipeline::Layer& BwFilter::setParam(const u32 param, std::any value) {
    switch (param) {
        SET_PARAM(cutoff);
        SET_PARAM(quality);
        SET_PARAM(gainDB);
        SET_PARAM(order);
    }
    refresh();
    return *this;
}

#define GET_PARAM(name) case FilterParams::name: result = _##name; break

    std::any BwFilter::getParam(const u32 param) {
    std::any result;

    switch (param) {
        GET_PARAM(cutoff);
        GET_PARAM(quality);
        GET_PARAM(gainDB);
        GET_PARAM(order);
    }

    return result;
}


}