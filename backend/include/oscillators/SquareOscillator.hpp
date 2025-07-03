#pragma once
#include <oscillators/PhaseOscillator.hpp>

class SquareOscillator : public PhaseOscillator {
public:
    using PhaseOscillator::PhaseOscillator;
    f32 getNextSample() override;
};