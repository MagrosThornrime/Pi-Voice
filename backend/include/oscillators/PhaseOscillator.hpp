#pragma once
#include <oscillators/Oscillator.hpp>

/// @brief An interface for all oscillators based on sinusoides
class PhaseOscillator : public Oscillator {
protected:
    /// @brief Current angle of the sinusoides
    f32 _phase = 0.0f;

public:
    /// @brief Constructor
    using Oscillator::Oscillator;

    /// @brief Updates the oscillator's state for the next frame
    void advance() override;

};