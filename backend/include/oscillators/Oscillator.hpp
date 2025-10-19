#pragma once
#include <portaudio.h>
#include <portaudiocpp/PortAudioCpp.hxx>

class Oscillator: public portaudio::CallbackInterface {
protected:
	float _amplitude = 1.0f;
	float _sampleRate;

	const float _REFERENCE_FREQUENCY = 220.0f;
	float _currentFrequency = _REFERENCE_FREQUENCY;

public:
	int paCallbackFun(const void* input, void* output,
		unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags) override;

	explicit Oscillator(float sampleRate);

	void setFrequency(int octave, int seminote);
	void setFrequency(float freq);
	void setAmplitude(float amplitude);

	virtual float getNextSample();
	virtual void advance() {};

};
