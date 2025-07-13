#pragma once
#include <oscillators/PhaseOscillator.hpp>

namespace oscillators {
/// @brief Generates simple sinusoide waves
class SineOscillator : public PhaseOscillator {
public:
    /// @brief Constructor
    using PhaseOscillator::PhaseOscillator;

    /// @brief Calculates next sample's value
    f32 getNextSample() override;
};
}
