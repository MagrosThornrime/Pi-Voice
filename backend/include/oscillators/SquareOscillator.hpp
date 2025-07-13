#pragma once
#include <oscillators/PhaseOscillator.hpp>

namespace oscillators {
/// @brief Generates square-like waves
class SquareOscillator : public PhaseOscillator {
public:
    /// @brief Constructor
    using PhaseOscillator::PhaseOscillator;

    /// @brief Calculates next sample's value
    f32 getNextSample() override;
};
}
