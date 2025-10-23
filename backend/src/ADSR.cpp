#include <ADSR.hpp>

void ADSR::reset(){
    _remainingAttack = attack;
    _remainingDecay = decay;
    _remainingRelease = release;
}

f32 ADSR::getAmplitude(bool isActive){
    f32 amplitude = 0.0f;
    if (_remainingAttack > 0){
        amplitude = 1.0f - (f32) _remainingAttack / (f32) attack;
        _remainingAttack--;
    }
    else if (_remainingDecay > 0){
        f32 decayFactor = 1.0f - (f32) _remainingDecay / (f32) decay;
        amplitude = 1.0f - (1.0f - sustain) * decayFactor;
        _remainingDecay--;
    }
    else if (isActive){
        amplitude = sustain;
    }
    else if (_remainingRelease > 0){
        amplitude = sustain * (f32) _remainingRelease / (f32) release;
        _remainingRelease--;
    }
    return amplitude;
}