#pragma once
#include <Voice.hpp>
#include <portaudiocpp/PortAudioCpp.hxx>


class VoiceManager : public portaudio::CallbackInterface {
    std::vector<Voice> _voices;
    f32 _getNextSample();

public:
    VoiceManager(i32 voicesNumber, f32 sampleRate);
	f32 amplitude = 0.1f;

    /// @brief PortAudio callback used for streaming the audio
    int paCallbackFun(const void* input, void* output,
        unsigned long frameCount,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags);

    void setOscillatorType(oscillators::OscillatorType type);
    void update();
	void turnOn(i32 voiceNumber);
    void turnOff(i32 voiceNumber);

	void setADSRAttack(f32 attack);
	void setADSRDecay(f32 decay);
	void setADSRSustain(f32 sustain);
	void setADSRRelease(f32 release);
};