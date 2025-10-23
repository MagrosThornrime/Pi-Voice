#pragma once
#include <Types.hpp>


class ADSR {

    i32 _remainingAttack{};
    i32 _remainingDecay{};
    i32 _remainingRelease{};

public:
    i32 attack = 10000;
    i32 decay = 1000;
    f32 sustain = 0.8f;
    i32 release = 100000;

    void reset();
    f32 getAmplitude(bool isActive);
};