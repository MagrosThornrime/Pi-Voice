#pragma once
#include <oscillators/Oscillator.hpp>
#include <random>

namespace oscillators {
/// @brief Generates noise-like sound (like in old TVs')
class NoiseOscillator : public Oscillator {
protected:
    /// @brief Current phase
    f32 _phase = 0.0f;

    /// @brief Last seed used by random generator
    u32 _lastSeed = 0;

    /// @brief Last sample generated
    f32 _lastValue = 0.0f;

    /// @brief Random number generator
    std::mt19937 _rng;

    /// @brief Used for generating numbers from the uniform distribution
    std::uniform_real_distribution<f32> _dist;

public:
    /// @brief Constructor
    explicit NoiseOscillator(f32 sampleRate);

    /// @brief Calculates next sample's value
    f32 getNextSample() override;

    /// @brief Updates the oscillator's state for the next frame
    void advance() override;

};
}

