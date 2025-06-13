#pragma once
#include <oscillators/Oscillator.hpp>
#include <random>

class NoiseOscillator : public Oscillator {
protected:
    float _phase = 0.0f;
    unsigned int _lastSeed = 0;
    float _lastValue = 0.0f;

    std::mt19937 _rng;
    std::uniform_real_distribution<float> _dist;

    void _advance() override;

public:
    explicit NoiseOscillator(float sampleRate);
    float getNextSample() override;
};
