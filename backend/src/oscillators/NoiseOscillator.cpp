#include <oscillators/NoiseOscillator.hpp>

namespace oscillators {
NoiseOscillator::NoiseOscillator(f32 sampleRate) : Oscillator(sampleRate) {
	_rng = std::mt19937(std::random_device{}());
	_dist = std::uniform_real_distribution<f32>(-1.0f, 1.0f);
}

void NoiseOscillator::advance(){
    _lastSeed = _phase;
    _phase += _currentFrequency / _sampleRate;
    while(_phase > 2.0f){
        _phase -= 1.0f;
    }
}

f32 NoiseOscillator::getNextSample() {
    u32 seed = _phase;
    if (seed == _lastSeed) {
        f32 value = _dist(_rng);
        _lastValue = value;
        return value;
    }
    return _lastValue;
}
}
