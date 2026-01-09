#include <oscillators/PhaseOscillator.hpp>
#include <numbers>
#include <cmath>

namespace oscillators{
PhaseOscillator::PhaseOscillator(f32 sampleRate, i32 voiceNumber) : Oscillator(sampleRate, voiceNumber) {
    setNote(voiceNumber);
}

void PhaseOscillator::advance() {
    _phase += 2.0 * std::numbers::pi * _currentFrequency / _sampleRate;
	_phase = std::fmod(_phase, 2.0 * std::numbers::pi);
}

void PhaseOscillator::reset() {
    _phase = 0.0f;
}

void PhaseOscillator::setNote(i32 voiceNumber){
    _voiceNumber = voiceNumber;
    _currentFrequency = 440.f * std::pow(2.f, (voiceNumber - 69.f) / 12.f);
}
}