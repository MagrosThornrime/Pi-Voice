#include <oscillators/SawtoothOscillator.hpp>
#include <cmath>
#include <numbers>

namespace oscillators {
f32 SawtoothOscillator::getNextSample() {
    //i32 harmonics = 0;
    //f32 partialFrequency = _currentFrequency;
    //while(partialFrequency < _sampleRate / 2.0f) {
       // harmonics++;
       // partialFrequency *= 2.0f;
    //}
	const i32 harmonics = 20;
	f32 sample = 0.0f;
    for(i32 i = 1; i <= harmonics; i++) {
        sample += std::sin(_phase * i) / i;
    }
    return sample * 2.0f / std::numbers::pi;
}
}