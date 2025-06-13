#include <oscillators/Oscillator.hpp>
#include <stdexcept>
#include <cmath>

int Oscillator::paCallback(const void *input, void *output,
               unsigned long frameCount,
               const PaStreamCallbackTimeInfo* timeInfo,
               PaStreamCallbackFlags statusFlags)
{
    float *out = (float*)output;
    for (int i = 0; i < frameCount; i++) {
		_advance();
        float sample = _amplitude * getNextSample();
        *out++ = sample;
        *out++ = sample;
    }
    return 0;
}

Oscillator::Oscillator(float sampleRate) : _sampleRate(sampleRate) {}

float Oscillator::getNextSample()  {
    return 0.0f;
}

void Oscillator::setFrequency(int octave, int seminote) {
    float distance = octave * 12 + seminote;
    _currentFrequency = _REFERENCE_FREQUENCY * std::pow(2.0f, distance / 12.0f);
}

void Oscillator::setAmplitude(float amplitude) {
	if(amplitude < 0.0f || amplitude > 1.0f) {
		throw std::invalid_argument("Amplitude must be between 0.0 and 1.0");
	}
    _amplitude = amplitude;
}