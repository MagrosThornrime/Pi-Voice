#include <oscillators/Oscillator.hpp>
#include <stdexcept>
#include <cmath>

namespace oscillators {

Oscillator::Oscillator(f32 sampleRate): _sampleRate(sampleRate) {}

f32 Oscillator::getNextSample() {
	return 0.0f;
}

void Oscillator::setFrequency(f32 freq) {
	_currentFrequency = freq;
}
}