#pragma once
#include <oscillators/Oscillator.hpp>

class PhaseOscillator : public Oscillator {
protected:
    float _phase = 0.0f;

public:
    using Oscillator::Oscillator;
    void advance() override;
};