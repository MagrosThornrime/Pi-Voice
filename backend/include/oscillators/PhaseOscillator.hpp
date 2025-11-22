#pragma once
#include <oscillators/Oscillator.hpp>

namespace oscillators{
/// @brief An interface for all oscillators based on sinusoides
class PhaseOscillator : public Oscillator {
protected:
    /// @brief Current angle of the sinusoides
    f32 _phase = 0.0f;

    /// @brief Frequency of generated sound
    f32 _currentFrequency = 440.0f;

public:

    /// @brief Constructor
    PhaseOscillator(f32 sampleRate, i32 voiceNumber);

    /// @brief Updates the oscillator's state for the next frame
    void advance() override;

};
}
