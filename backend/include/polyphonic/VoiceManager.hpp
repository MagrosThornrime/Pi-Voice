#pragma once
#include "Voice.hpp"
#include <pipeline/Layer.hpp>
#include <mutex>
#include <vector>

namespace polyphonic {
/// @brief Controls all Voices in the synthesiser and mixes their outputs
class VoiceManager {
	/// @brief Used Voices
	std::vector<Voice> _voices;

	/// @brief Gets current sample of the sound
	f32 _getNextSample();

	/// @brief Mutex for changing oscillators
	std::mutex _oscillatorMutex;

	/// @brief Sound's sampling rate
	f32 _sampleRate;

	/// @brief Channels number
	u32 _channels;

	/// @brief Oscillator types of the next sound
	std::string _oscillatorTypes[3] = {"empty", "empty", "empty"};

	/// @brief Attack factor value of the next sound
	f32 _attack = 0.001f;

	/// @brief Decay factor value of the next sound
	f32 _decay = 0.02f;

	/// @brief Sustain amplitude value of the next sound
	f32 _sustain = 0.8f;

	/// @brief Release value of the next sound
	f32 _release = 0.00001f;

	/// @brief Find an unused voice. Returns -1 if not found.
	i32 _findVoice();

public:
	/// @brief Constructor
	/// @param voicesNumber the number of notes used by keyboard
	/// @param sampleRate sound's sample rate
	/// @param channels channels number
	/// @param sampleManager sample manager
	VoiceManager(i32 voicesNumber, f32 sampleRate, u32 channels,
		 std::shared_ptr<fileio::SampleManager> sampleManager);

	/// @brief Callback used for streaming the audio
	/// @param out output buffer
	/// @param frames number of frames generated
	void generateSound(std::vector<f32>& out, u32 frames);

	/// @brief Replaces oscillators of all Voices
	/// @param type type of the new oscillator
	/// @param index id of the new oscillator (0, 1 or 2)
	void setOscillatorType(const std::string& type, i32 index);

	/// @brief Advance all oscillators
	void update();

	/// @brief Signalise that a key was pressed
	/// @param voiceNumber ID of the key
	void turnOn(i32 voiceNumber);

	/// @brief Signalise that a key was released
	/// @param voiceNumber ID of the key
	void turnOff(i32 voiceNumber);

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

	/// @brief Tells if at least one voice is turned on
	bool hasActiveVoices();
};
}
