#pragma once
#include <oscillators/Oscillator.hpp>

class PhaseOscillator : public Oscillator {
protected:
    f32 _phase = 0.0f;

public:
    using Oscillator::Oscillator;
    void advance() override;
};