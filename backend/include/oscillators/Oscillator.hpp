#pragma once
#include <portaudio.h>
#include <Types.hpp>

namespace oscillators {
/// @brief An interface for all oscillators
class Oscillator {
protected:
    /// @brief Sample rate of generated sound
    f32 _sampleRate;

    /// @brief Frequency of generated sound
    f32 _currentFrequency = 440.0f;

public:
    /// @brief Constructor
    explicit Oscillator(f32 sampleRate);

    /// @brief Explicitly sets the frequency
    void setFrequency(f32 freq);

    /// @brief Updates the oscillator's state for the next frame
    virtual void advance() {};

    /// @brief Calculates next sample's value
    virtual f32 getNextSample();

};
}
