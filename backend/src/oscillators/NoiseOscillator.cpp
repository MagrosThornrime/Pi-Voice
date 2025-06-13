#include <oscillators/NoiseOscillator.hpp>


NoiseOscillator::NoiseOscillator(float sampleRate) : Oscillator(sampleRate) {
	_rng = std::mt19937(std::random_device{}());
	_dist = std::uniform_real_distribution<float>(-1.0f, 1.0f);
}

void NoiseOscillator::advance(){
    _lastSeed = _phase;
    _phase += _currentFrequency / _sampleRate;
    while(_phase > 2.0f){
        _phase -= 1.0f;
    }
}

float NoiseOscillator::getNextSample() {
    unsigned int seed = _phase;
    if (seed == _lastSeed) {
        float value = _dist(_rng);
        _lastValue = value;
        return value;
    }
    return _lastValue;
}