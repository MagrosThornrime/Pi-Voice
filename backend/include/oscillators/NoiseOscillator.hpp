#pragma once
#include <oscillators/Oscillator.hpp>
#include <random>

class NoiseOscillator : public Oscillator {
protected:
    f32 _phase = 0.0f;
    u32 _lastSeed = 0;
    f32 _lastValue = 0.0f;

    std::mt19937 _rng;
    std::uniform_real_distribution<f32> _dist;

public:
    explicit NoiseOscillator(f32 sampleRate);
    f32 getNextSample() override;
    void advance() override;
};
