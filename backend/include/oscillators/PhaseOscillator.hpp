#pragma once
#include <oscillators/Oscillator.hpp>

class PhaseOscillator : public Oscillator {
protected:
    float _phase = 0.0f;

    void _advance() override;

public:
    using Oscillator::Oscillator;
};