#pragma once
#include <Voice.hpp>
#include <portaudiocpp/PortAudioCpp.hxx>
#include <mutex>

/// @brief Controls all Voices in the synthesiser and mixes their outputs
class VoiceManager : public portaudio::CallbackInterface {
	/// @brief Used Voices
    std::vector<Voice> _voices;

	/// @brief Gets current sample of the sound
    f32 _getNextSample();

	/// @brief Mutex for changing oscillators
	std::mutex _oscillatorMutex;

public:
	/// @brief Constructor
	/// @param voicesNumber the number of notes used by keyboard
	/// @param sampleRate sound's sample rate
    VoiceManager(i32 voicesNumber, f32 sampleRate);

	/// @brief Global amplitude
	f32 amplitude = 0.1f;

    /// @brief PortAudio callback used for streaming the audio
    int paCallbackFun(const void* input, void* output,
        unsigned long frameCount,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags);

	/// @brief Replaces oscillators of all Voices
	/// @param type type of the new oscillator
    void setOscillatorType(oscillators::OscillatorType type);

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
};