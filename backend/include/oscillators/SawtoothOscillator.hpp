#pragma once
#include <oscillators/PhaseOscillator.hpp>

class SawtoothOscillator : public PhaseOscillator {
public:
    using PhaseOscillator::PhaseOscillator;
    f32 getNextSample() override;
};