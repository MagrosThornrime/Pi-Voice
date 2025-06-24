#include <oscillators/SawtoothOscillator.hpp>
#include <cmath>
#include <numbers>

float SawtoothOscillator::getNextSample() {
    int harmonics = 0;
    float partialFrequency = _currentFrequency;
    while(partialFrequency < _sampleRate / 2.0f) {
        harmonics++;
        partialFrequency *= 2.0f;
    }
    float sample = 0.0f;
    for(int i = 1; i <= harmonics; i++) {
        sample += std::sin(_phase * (float) i) / (float) i;
    }
    return sample * 2.0f / std::numbers::pi;
}