#pragma once
#include <Oscillators.hpp>
#include <Types.hpp>
#include <memory>
#include "ADSR.hpp"
#include <fileio/SampleManager.hpp>

namespace polyphonic {
/// @brief A source of sounds played by a particular key
class Voice {
	/// @brief Current oscillators
    std::shared_ptr<oscillators::Oscillator> _oscillators[3];

	/// @brief The note's index
	i32 _voiceNumber;

	/// @brief Sound sample rate
	f32 _sampleRate;

	/// @brief Calculates amplitude using ADSR envelope
    ADSR _adsr;

	/// @brief Loads samples to be used as oscillators
	std::shared_ptr<fileio::SampleManager> _sampleManager;

	/// @brief Tells how many non empty oscillators are used
	u32 _nonEmpty = 0;

public:

	/// @brief Tells if the key is pressed
	bool isPressed = false;

	/// @brief Constructor
	/// @param sampleRate sound sample rate
	/// @param sampleManager sample manager
    Voice(f32 sampleRate, std::shared_ptr<fileio::SampleManager> sampleManager);

	/// @brief Replaces a current oscillator
	/// @param oscillatorType type of the new oscillator
	/// @param index id of the oscillator (0, 1 or 2)
    void setOscillatorType(const std::string& oscillatorType, i32 index);

	/// @brief Gets current sample of the sound
    f32 getNextSample();

	/// @brief Advance the oscillators
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

	/// @brief Set music note
	/// @param note note of the sound
	void setNote(i32 note);

	/// @brief Get music note
	i32 getNote();

	/// @brief Tells if the sound is playing
	bool isPlaying();
};
}