#pragma once
#include <Oscillators.hpp>
#include <Types.hpp>
#include <memory>
#include "ADSR.hpp"

/// @brief A source of sounds played by a particular key
class Voice {
	/// @brief Current oscillator
    std::shared_ptr<oscillators::Oscillator> _oscillator;

	/// @brief The note's frequency
	f32 _frequency;

	/// @brief Sound sample rate
	f32 _sampleRate;

	/// @brief Calculates amplitude using ADSR envelope
    ADSR _adsr;

public:

	/// @brief Tells if the key is pressed
	bool isActive = false;

	/// @brief Constructor
	/// @param voiceNumber ID of the note
	/// @param sampleRate sound sample rate
    Voice(i32 voiceNumber, f32 sampleRate);

	/// @brief Replaces the current oscillator
	/// @param oscillatorType type of the new oscillator
    void setOscillatorType(oscillators::OscillatorType oscillatorType);

	/// @brief Gets current sample of the sound
    f32 getNextSample();

	/// @brief Advance the oscillator
    void update();

	/// @brief Signalize that you pressed the key
	void turnOn();

	/// @brief Signalize that you released the key
    void turnOff();

	/// @brief Set attack value for ADSR
	/// @param attack rate of the amplitude's increase after pressing the key (between 0.0 and 1.0)
	void setAttack(f32 attack);

	/// @brief Set decay value for ADSR
	/// @param decay rate of the amplitude's decrease after reaching 1.0 (between 0.0 and 1.0)
	void setDecay(f32 decay);

	/// @brief Set sustain value for ADSR
	/// @param sustain amplitude used when the key is still being hold (between 0.0 and 1.0)
	void setSustain(f32 sustain);

	/// @brief Set release value for ADSR
	/// @param release rate of the amplitude's decrease after releasing the key (between 0.0 and 1.0)
	void setRelease(f32 release);
};