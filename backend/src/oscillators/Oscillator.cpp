#include <oscillators/Oscillator.hpp>
#include <stdexcept>
#include <cmath>

namespace oscillators {

Oscillator::Oscillator(f32 sampleRate, i32 voiceNumber):
	_sampleRate(sampleRate), _voiceNumber(voiceNumber) {}

f32 Oscillator::getNextSample() {
	return 0.0f;
}
}