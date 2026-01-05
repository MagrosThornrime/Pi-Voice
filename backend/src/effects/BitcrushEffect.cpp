#include <effects/BitcrushEffect.hpp>
#include <cmath>

namespace effects {
    void BitcrushEffect::processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames){
        for(u32 i = 0; i < frames * _channels; i++){
            outputBuffer[i] = std::round(inputBuffer[i] * _levels) / _levels;
        }
    }

#define SET_PARAM(name) case BitcrushParams::name: if(value.type() == typeid(_##name)) { _##name = std::any_cast<decltype(_##name)>(std::move(value)); } break

    pipeline::Layer& BitcrushEffect::setParam(const u32 param, std::any value){
        switch (param) {
            SET_PARAM(bits);
        }
        refresh();
        return *this;
    }

#define GET_PARAM(name) case BitcrushParams::name: result = _##name; break

    std::any BitcrushEffect::getParam(const u32 param){
        std::any result;

        switch (param) {
            GET_PARAM(bits);
        }

        return result;
    }

    BitcrushEffect::BitcrushEffect(const u32 channels, const f32 sampleRate) : Effect(channels, sampleRate) {
        refresh();
    }

    void BitcrushEffect::refresh(){
        _levels = 1 << _bits;
    }

    EffectType::Value BitcrushEffect::getEffectType(){
        return EffectType::bitcrush;
    }

}