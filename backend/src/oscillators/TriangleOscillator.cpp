#include <oscillators/TriangleOscillator.hpp>
#include <cmath>
#include <numbers>
#include <algorithm>

namespace oscillators {
f32 TriangleOscillator::getNextSample() {
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
    sample *= 8.0f / std::pow(std::numbers::pi, 2);
    return std::clamp(sample, 0.0f, 1.0f);
}
}