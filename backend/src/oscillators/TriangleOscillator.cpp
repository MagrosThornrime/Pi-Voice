#include <oscillators/TriangleOscillator.hpp>
#include <cmath>
#include <numbers>

namespace oscillators {
f32 TriangleOscillator::getNextSample() {
    //i32 harmonics = 0;
    //while(_currentFrequency * (f32) (harmonics * 2 - 1) < _sampleRate / 2.0f) {
    //    harmonics++;
    //}
    //harmonics--;
    const i32 harmonics = 20;
    f32 sample = 0.0f;
    bool subtract = true;
    for(i32 i = 1; i <= harmonics; i++) {
        f32 factor = i * 2 - 1;
        f32 samplePart = std::sin(_phase * factor) / factor;
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
}