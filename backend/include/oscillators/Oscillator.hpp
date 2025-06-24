#pragma once
#include <portaudio.h>

class Oscillator {
protected:
	float _amplitude = 1.0f;
	float _sampleRate;

	const float _REFERENCE_FREQUENCY = 220.0f;
	float _currentFrequency = _REFERENCE_FREQUENCY;

public:
	int paCallback(const void* input, void* output,
		unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags);

	explicit Oscillator(float sampleRate);

	void setFrequency(int octave, int seminote);
	void setFrequency(float freq);
	void setAmplitude(float amplitude);

	virtual float getNextSample();
	virtual void advance() {};

};
