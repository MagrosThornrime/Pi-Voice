#pragma once
#include <oscillators/PhaseOscillator.hpp>

class SineOscillator : public PhaseOscillator {
public:
    using PhaseOscillator::PhaseOscillator;
    float getNextSample() override;
};