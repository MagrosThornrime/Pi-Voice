#include <oscillators/SquareOscillator.hpp>
#include <cmath>
#include <numbers>

float SquareOscillator::getNextSample() {
    int harmonics = 0;
    while(_currentFrequency * (float) (harmonics * 2 - 1) < _sampleRate / 2.0f) {
        harmonics++;
    }
    harmonics--;
    float sample = 0.0f;
    for(int i = 1; i <= harmonics; i++) {
        float factor = i * 2 - 1;
        sample += std::sin(_phase * factor) / factor;
    }
    return sample * 4.0f / std::numbers::pi;
}