#pragma once
#include <portaudio.h>
#include <Types.hpp>

namespace oscillators {
/// @brief An interface for all oscillators
class Oscillator {
protected:
    /// @brief Amplitude of generated sound
    f32 _amplitude = 1.0f;

    /// @brief Sample rate of generated sound
    f32 _sampleRate;

    /// @brief Frequency of generated sound
    f32 _currentFrequency = _REFERENCE_FREQUENCY;

public:
    /// @brief PortAudio callback used for streaming the audio
    i32 paCallback(const void* input, void* output,
        u64 frameCount,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags);

    /// @brief Constructor
    explicit Oscillator(f32 sampleRate);

    /// @brief Explicitly sets the frequency
    void setFrequency(f32 freq);

    /// @brief Sets amplitude
    void setAmplitude(f32 amplitude);

    /// @brief Updates the oscillator's state for the next frame
    virtual void advance() {};

    /// @brief Calculates next sample's value
    virtual f32 getNextSample();

};
}
