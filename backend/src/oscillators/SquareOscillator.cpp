#include <oscillators/SquareOscillator.hpp>
#include <cmath>
#include <numbers>
#include <algorithm>

namespace oscillators {
f32 SquareOscillator::getNextSample() {
    const i32 harmonics = 20;
    f32 sample = 0.0f;
    for(i32 i = 1; i <= harmonics; i++) {
        f32 factor = i * 2 - 1;
        sample += std::sin(_phase * factor) / factor;
    }
    sample *= 4.0f / std::numbers::pi;
    return std::clamp(sample, 0.0f, 1.0f);
}

bool SquareOscillator::isEmpty() {
    return false;
}
}