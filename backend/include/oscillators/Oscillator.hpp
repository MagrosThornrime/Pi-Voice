#pragma once
#include <portaudio.h>
#include <Types.hpp>

class Oscillator {
protected:
	f32 _amplitude = 1.0f;
	f32 _sampleRate;

	const f32 _REFERENCE_FREQUENCY = 220.0f;
	f32 _currentFrequency = _REFERENCE_FREQUENCY;

public:
	i32 paCallback(const void* input, void* output,
		u64 frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags);

	explicit Oscillator(f32 sampleRate);

	void setFrequency(i32 octave, i32 seminote);
	void setFrequency(f32 freq);
	void setAmplitude(f32 amplitude);

	virtual f32 getNextSample();
	virtual void advance() {};

};
