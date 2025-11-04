#include <oscillators/SquareOscillator.hpp>
#include <cmath>
#include <numbers>

namespace oscillators {
f32 SquareOscillator::getNextSample() {
    //i32 harmonics = 0;
    //while(_currentFrequency * (f32) (harmonics * 2 - 1) < _sampleRate / 2.0f) {
    //    harmonics++;
    //}
    //harmonics--;
    const i32 harmonics = 8;
    f32 sample = 0.0f;
    for(i32 i = 1; i <= harmonics; i++) {
        f32 factor = i * 2 - 1;
        sample += std::sin(_phase * factor) / factor;
    }
    return sample * 4.0f / std::numbers::pi;
}
}