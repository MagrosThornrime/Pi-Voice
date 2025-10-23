#pragma once
#include <Types.hpp>

enum ADSRStage{
	attack, decay, sustain, release
};

class ADSR {

    f32 _amplitude{};
	ADSRStage _stage = release;

public:
    f32 attackFactor = 1.0f;
    f32 decayFactor = 0.2f;
    f32 sustainAmplitude = 0.8f;
    f32 releaseFactor = 0.00001f;

    void reset();
    f32 getAmplitude(bool isActive);
};