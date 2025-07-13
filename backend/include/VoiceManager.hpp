#pragma once
#include <Voice.hpp>

class VoiceManager {
    std::vector<Voice> _voices;

public:
    explicit VoiceManager(i32 voicesNumber);

    void setAmplitude(f32 amplitude);
    void setOscillatorType(OscillatorType type);
    f32 getNextSample();
    void update();
};