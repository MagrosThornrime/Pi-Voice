#pragma once
#include <oscillators/Oscillator.hpp>

class SineOscillator : public Oscillator {
    float _phase;
public:
    using Oscillator::Oscillator;
    float getNextSample() override;
};