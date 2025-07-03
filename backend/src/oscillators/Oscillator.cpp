#include <oscillators/Oscillator.hpp>
#include <stdexcept>
#include <cmath>

i32 Oscillator::paCallback(const void* input, void* output,
	u64 frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags) {
	f32* out = (f32*)output;
	for (i32 i = 0; i < frameCount; i++) {
		advance();
		f32 sample = _amplitude * getNextSample();
		*out++ = sample;
		*out++ = sample;
	}
	return 0;
}

Oscillator::Oscillator(f32 sampleRate): _sampleRate(sampleRate) {}

f32 Oscillator::getNextSample() {
	return 0.0f;
}

void Oscillator::setFrequency(i32 octave, i32 seminote) {
	f32 distance = octave * 12 + seminote;
	_currentFrequency = _REFERENCE_FREQUENCY * std::pow(2.0f, distance / 12.0f);
}

void Oscillator::setFrequency(f32 freq) {
	_currentFrequency = freq;
}

void Oscillator::setAmplitude(f32 amplitude) {
	if (amplitude < 0.0f || amplitude > 1.0f) {
		throw std::invalid_argument("Amplitude must be between 0.0 and 1.0");
	}
	_amplitude = amplitude;
}