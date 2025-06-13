#include <oscillators/SineOscillator.hpp>
#include <numbers>
#include <cmath>


float SineOscillator::getNextSample() {
    _phase += 2.0 * std::numbers::pi * _currentFrequency / _sampleRate;
    while(_phase >= 2.0 * std::numbers::pi) {
        _phase -= 2.0 * std::numbers::pi;
    }
    while(_phase < 0.0) {
        _phase += 2.0 * std::numbers::pi;
    }
    return (1.0f + std::sin(_phase)) / 2.0f;
}