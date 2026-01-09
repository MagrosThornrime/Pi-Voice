#pragma once
#include <oscillators/PhaseOscillator.hpp>

namespace oscillators {
/// @brief Generates triangle-like waves
class TriangleOscillator : public PhaseOscillator {
public:
    /// @brief Constructor
    using PhaseOscillator::PhaseOscillator;

    /// @brief Calculates next sample's value
    f32 getNextSample() override;

    /// @brief Tells if it's an empty oscillator
    bool isEmpty() override;
};
}
