#pragma once
#include <Types.hpp>

namespace polyphonic {
// @brief Stages on ADSR envelope
enum ADSRStage{
    stopped, ///< when the music doesn't play
	attack, ///< when the key is pressed, the amplitude starts to grow until it reaches 1.0
    decay, ///< after the amplitude reaches 1.0, it declines to the sustain value
    sustain, ///< while the key is being hold, the amplitude is constant
    release ///< after releasing the key, the amplitude declines until it reaches 0.0
};

/// @brief Calculates current amplitude using ADSR envelope
class ADSR {

    /// @brief Current amplitude
    f32 _amplitude{};

    // @brief Current stage on the envelope
	ADSRStage _stage = stopped;

public:
    // @brief Tells how fast the amplitude grows in the attack stage
    f32 attackFactor = 0.001f;

    /// @brief Tells how fast the amplitude declines in the decay stage
    f32 decayFactor = 0.02f;

    /// @brief Amplitude value during sustain stage
    f32 sustainAmplitude = 0.8f;

    /// @brief Tells how fast the amplitude declines in the release stage
    f32 releaseFactor = 0.00001f;

    /// @brief Reset current amplitude and stage when a key is pressed
    void reset();

    /// @brief Get current amplitude
    /// @param isPressed tells if the Voice using this is pressed
    f32 getAmplitude(bool isPressed);

    /// @brief Tells if the sound is playing or it's stopped
    bool isPlaying();
};
}