#include <oscillators/ModulatedOscillator.hpp>
#include <cmath>

namespace oscillators {
ModulatedOscillator::ModulatedOscillator(f32 sampleRate, i32 voiceNumber, const std::vector<f32>& sample)
    : Oscillator(sampleRate, voiceNumber), _sample(sample) {
    _stepFactor = std::pow(2.f, (voiceNumber - 69.f) / 12.f);
}

void ModulatedOscillator::advance() {
    _index += _stepFactor;
    while(_index >= _sample.size()) {
        _index -= _sample.size();
    }
}

f32 ModulatedOscillator::getNextSample() {
    return _sample[_index];
}

void ModulatedOscillator::reset() {
    _index = 0;
}
}