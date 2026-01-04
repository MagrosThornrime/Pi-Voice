#include <effects/SaturateEffect.hpp>
#include <cmath>

namespace effects {
    void SaturateEffect::processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames){
        for(u32 i = 0; i < frames * _channels; i++){
            outputBuffer[i] = std::tanh(inputBuffer[i] * _drive);
        }
    }

#define SET_PARAM(name) case SaturateParams::name: if(value.type() == typeid(_##name)) { _##name = std::any_cast<decltype(_##name)>(std::move(value)); } break

    pipeline::Layer& SaturateEffect::setParam(const u32 param, std::any value){
        switch (param) {
            SET_PARAM(drive);
        }
        refresh();
        return *this;
    }

#define GET_PARAM(name) case SaturateParams::name: result = _##name; break

    std::any SaturateEffect::getParam(const u32 param){
        std::any result;

        switch (param) {
            GET_PARAM(drive);
        }

        return result;
    }

    SaturateEffect::SaturateEffect(const u32 channels, const f32 sampleRate) : Effect(channels, sampleRate) {
        refresh();
    }

    void SaturateEffect::refresh(){}

    EffectType::Value SaturateEffect::getEffectType(){
        return EffectType::saturate;
    }

}