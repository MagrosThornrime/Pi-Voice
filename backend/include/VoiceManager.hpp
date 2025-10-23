#pragma once
#include <Voice.hpp>

class VoiceManager {
    std::vector<Voice> _voices;
    f32 _getNextSample();

public:
    VoiceManager(i32 voicesNumber, f32 sampleRate);
	f32 amplitude = 0.1f;

    /// @brief PortAudio callback used for streaming the audio
    i32 paCallback(const void* input, void* output,
        u64 frameCount,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags);

    void setOscillatorType(oscillators::OscillatorType type);
    void update();
	void turnOn(i32 voiceNumber);
    void turnOff(i32 voiceNumber);
};