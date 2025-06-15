#include <oscillators/TriangleOscillator.hpp>
#include <cmath>
#include <numbers>


float TriangleOscillator::getNextSample() {
    int harmonics = 0;
    while(_currentFrequency * (float) (harmonics * 2 - 1) < _sampleRate / 2.0f) {
        harmonics++;
    }
    harmonics--;
    float sample = 0.0f;
    bool subtract = true;
    for(int i = 1; i <= harmonics; i++) {
        float factor = i * 2 - 1;
        float samplePart = std::sin(_phase * factor) / factor;
        if(subtract) {
            sample -= samplePart;
        }
        else{
            sample += samplePart;
        }
        subtract = not subtract;
    }
    return sample * 8.0f / std::pow(std::numbers::pi, 2);
}