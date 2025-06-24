#pragma once
#include <oscillators/PhaseOscillator.hpp>

class TriangleOscillator : public PhaseOscillator {
public:
    using PhaseOscillator::PhaseOscillator;
    float getNextSample() override;
};