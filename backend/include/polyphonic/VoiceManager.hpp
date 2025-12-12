#pragma once
#include "Voice.hpp"
#include <pipeline/Layer.hpp>
#include <mutex>
#include <vector>

namespace polyphonic {
/// @brief Controls all Voices in the synthesiser and mixes their outputs
class VoiceManager: public pipeline::Layer {
	/// @brief Used Voices
	std::vector<Voice> _voices;

	/// @brief Gets current sample of the sound
	f32 _getNextSample();

	/// @brief Mutex for changing oscillators
	std::mutex _oscillatorMutex;

	/// @brief Global amplitude
	f32 _amplitude = 0.1f;

	/// @brief Sound's sampling rate
	f32 _sampleRate;

	/// @brief Oscillator types of the next sound
	std::string _oscillatorTypes[3] = {"empty", "empty", "empty"};

	i32 _nonEmpty = 0;

	/// @brief Oscillator amplitudes of the next sound
	f32 _oscillatorAmplitudes[3] = {1.0f, 1.0f, 1.0f};

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
	/// @param sampleManager sample manager
	VoiceManager(i32 voicesNumber, f32 sampleRate, std::shared_ptr<fileio::SampleManager> sampleManager);

	/// @brief PortAudio callback used for streaming the audio
	int paCallbackFun(const void* input, void* output,
		unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags);

	/// @brief Change the global amplitude
	/// @param amplitude new global amplitude (between 0 and 1)
	void setAmplitude(f32 amplitude);

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

	// dummy Layer methods
	pipeline::Layer& setParam(const u32 param, std::any value);
	std::any getParam(const u32 param);
};
}