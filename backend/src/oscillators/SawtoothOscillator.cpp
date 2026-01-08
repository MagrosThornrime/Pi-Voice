#include <oscillators/SawtoothOscillator.hpp>
#include <cmath>
#include <numbers>
#include <algorithm>

namespace oscillators {
f32 SawtoothOscillator::getNextSample() {
	const i32 harmonics = 8;
	f32 sample = 0.0f;
    for(i32 i = 1; i <= harmonics; i++) {
        sample += std::sin(_phase * i) / i;
    }
    sample *= 2.0f / std::numbers::pi;
    return std::clamp(sample, 0.0f, 1.0f);
}

bool SawtoothOscillator::isEmpty() {
    return false;
}
}