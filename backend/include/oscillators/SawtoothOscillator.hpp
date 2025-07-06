#pragma once
#include <oscillators/PhaseOscillator.hpp>

/// @brief Generates sawtooth-like waves
class SawtoothOscillator : public PhaseOscillator {
public:
    /// @brief Constructor
    using PhaseOscillator::PhaseOscillator;

    /// @brief Calculates next sample's value
    f32 getNextSample() override;
};