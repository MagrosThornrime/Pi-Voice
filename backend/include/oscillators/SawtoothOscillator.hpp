#pragma once
#include <oscillators/PhaseOscillator.hpp>

class SawtoothOscillator : public PhaseOscillator {
public:
    using PhaseOscillator::PhaseOscillator;
    float getNextSample() override;
};