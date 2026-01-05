#include <effects/PhaserEffect.hpp>
#include <numbers>
#include <cmath>

namespace effects {
    void PhaserEffect::processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames){
        u32 idx = 0;
        for(u32 i = 0; i < frames; i++){
            f32 lfoValue = (std::sin(_lfoPhase) + 1.0f) / 2.0;
            _lfoPhase += _lfoIncrement;
            _lfoPhase = std::fmod(_lfoPhase, 2.0f * std::numbers::pi);

            f32 coeff = _minCoeff + (_maxCoeff - _minCoeff) * lfoValue;

            for(auto& stage : _allPassStages){
                stage.setCoefficient(coeff);
            }

            for(u32 j = 0; j < _channels; j++, idx++){
                f32 y = inputBuffer[idx] * (1 - _feedback) + _feedbackBuffer[j] * _feedback;

                for(auto& stage : _allPassStages){
                    y = stage.processSample(y);
                }

                _feedbackBuffer[j] = y;
                outputBuffer[idx] = inputBuffer[idx] * (1 - _mix) + y * _mix;
            }
        }

    }

#define SET_PARAM(name) case PhaserParams::name: if(value.type() == typeid(_##name)) { _##name = std::any_cast<decltype(_##name)>(std::move(value)); } break

    pipeline::Layer& PhaserEffect::setParam(const u32 param, std::any value){
        switch (param) {
            SET_PARAM(minHz);
            SET_PARAM(rangeHz);
            SET_PARAM(rateHz);
            SET_PARAM(feedback);
            SET_PARAM(mix);
        }
        refresh();
        return *this;
    }

#define GET_PARAM(name) case PhaserParams::name: result = _##name; break

    std::any PhaserEffect::getParam(const u32 param){
        std::any result;

        switch (param) {
            GET_PARAM(minHz);
            GET_PARAM(rangeHz);
            GET_PARAM(rateHz);
            GET_PARAM(feedback);
            GET_PARAM(mix);
        }

        return result;
    }

    PhaserEffect::PhaserEffect(const u32 channels, const f32 sampleRate)
        : Effect(channels, sampleRate) {
        _feedbackBuffer.resize(channels);
        refresh();
    }

    void PhaserEffect::refresh(){
        _lfoPhase = 0.0f;

        f32 maxHz = _minHz + _rangeHz;
        _minCoeff = _minHz / _sampleRate * 0.5f;
        _maxCoeff = maxHz / _sampleRate * 0.5f;

        _lfoIncrement = 2.0f * std::numbers::pi * _rateHz / _sampleRate;
    }

    EffectType::Value PhaserEffect::getEffectType(){
        return EffectType::phaser;
    }

}