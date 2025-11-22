#include <oscillators/PhaseOscillator.hpp>
#include <numbers>
#include <cmath>

namespace oscillators{
PhaseOscillator::PhaseOscillator(f32 sampleRate, i32 voiceNumber) : Oscillator(sampleRate, voiceNumber) {
    _currentFrequency = 440.f * std::pow(2.f, (voiceNumber - 69.f) / 12.f);
}

void PhaseOscillator::advance() {
    _phase += 2.0 * std::numbers::pi * _currentFrequency / _sampleRate;
    while(_phase > 2.0 * std::numbers::pi) {
        _phase -= 2.0 * std::numbers::pi;
    }
    while(_phase < 0.0) {
        _phase += 2.0 * std::numbers::pi;
    }
}
}