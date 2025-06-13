#include <oscillators/PhaseOscillator.hpp>
#include <numbers>

void PhaseOscillator::_advance() {
    _phase += 2.0 * std::numbers::pi * _currentFrequency / _sampleRate;
    while(_phase > 2.0 * std::numbers::pi) {
        _phase -= 2.0 * std::numbers::pi;
    }
    while(_phase < 0.0) {
        _phase += 2.0 * std::numbers::pi;
    }
}