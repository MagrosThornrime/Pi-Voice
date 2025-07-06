#pragma once
#include <portaudio.h>
#include <Types.hpp>

/// @brief An interface for all oscillators
class Oscillator {
protected:
	/// @brief Amplitude of generated sound
	f32 _amplitude = 1.0f;

	/// @brief Sample rate of generated sound
	f32 _sampleRate;

	/// @brief A base frequency used for calculating semitones' frequencies
	const f32 _REFERENCE_FREQUENCY = 220.0f;

	/// @brief Frequency of generated sound
	f32 _currentFrequency = _REFERENCE_FREQUENCY;

public:
	/// @brief PortAudio callback used for streaming the audio
	i32 paCallback(const void* input, void* output,
		u64 frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags);

	/// @brief Constructor
	explicit Oscillator(f32 sampleRate);

	/// @brief Sets the frequency to one assigned to given note
	void setFrequency(i32 octave, i32 seminote);

	/// @brief Explicitly sets the frequency
	void setFrequency(f32 freq);

	/// @brief Sets amplitude
	void setAmplitude(f32 amplitude);

	/// @brief Updates the oscillator's state for the next frame
	virtual void advance() {};

	/// @brief Calculates next sample's value
	virtual f32 getNextSample();

};
