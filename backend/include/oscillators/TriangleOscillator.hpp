#pragma once
#include <oscillators/PhaseOscillator.hpp>

class TriangleOscillator : public PhaseOscillator {
public:
    using PhaseOscillator::PhaseOscillator;
    f32 getNextSample() override;
};