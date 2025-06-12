#include <oscillators/Oscillator.hpp>

int Oscillator::paCallback(const void *input, void *output,
               unsigned long frameCount,
               const PaStreamCallbackTimeInfo* timeInfo,
               PaStreamCallbackFlags statusFlags)
{
    float *out = (float*)output;
    for (int i = 0; i < frameCount; i++) {
        float phase = getNextPhase();
        *out++ = phase;
        *out++ = phase;
    }
    return 0;
}

float Oscillator::getNextPhase()  {
    return _currentPhase++;
}