#pragma once
#include <portaudio.h>

class Oscillator {
protected:
    float _currentPhase = 0.0f;
public:
    int paCallback(const void *input, void *output,
                   unsigned long frameCount,
                   const PaStreamCallbackTimeInfo* timeInfo,
                   PaStreamCallbackFlags statusFlags);

    virtual float getNextPhase();
};
