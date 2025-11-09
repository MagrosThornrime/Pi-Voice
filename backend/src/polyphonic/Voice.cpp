#include <polyphonic/Voice.hpp>
#include <fmt/core.h>

namespace polyphonic {
void Voice::setOscillatorType(oscillators::OscillatorType oscillatorType, i32 index) {
    if(index < 0 || index > 2){
        throw std::invalid_argument(fmt::format("Invalid index value of: {}", index));
    }
    switch (oscillatorType) {
        case oscillators::empty:
            _oscillators[index] = std::make_shared<oscillators::Oscillator>(_sampleRate);
            break;
        case oscillators::noise:
            _oscillators[index] = std::make_shared<oscillators::NoiseOscillator>(_sampleRate);
            break;
        case oscillators::sine:
            _oscillators[index] = std::make_shared<oscillators::SineOscillator>(_sampleRate);
            break;
        case oscillators::sawtooth:
            _oscillators[index] = std::make_shared<oscillators::SawtoothOscillator>(_sampleRate);
            break;
        case oscillators::square:
            _oscillators[index] = std::make_shared<oscillators::SquareOscillator>(_sampleRate);
            break;
        case oscillators::triangle:
            _oscillators[index] = std::make_shared<oscillators::TriangleOscillator>(_sampleRate);
            break;
    }
	_oscillators[index]->setFrequency(_frequency);
}

f32 Voice::getNextSample(){
    f32 sample = 0.0f;
    for (i32 i=0; i<3; i++){
        sample += _amplitudes[i] * _oscillators[i]->getNextSample();
    }
	return _adsr.getAmplitude(isPressed) * sample;
}

void Voice::update(){
    for (i32 i=0; i<3; i++){
        _oscillators[i]->advance();
    }
}

void Voice::setOscillatorAmplitude(f32 amplitude, i32 index){
    if(index < 0 || index > 2){
        throw std::invalid_argument(fmt::format("Invalid index value of: {}", index));
    }
    if(amplitude < 0.0f || amplitude > 1.0f){
        throw std::invalid_argument(fmt::format("Invalid amplitude value of: {}", amplitude));
    }
    _amplitudes[index] = amplitude;
}

Voice::Voice(i32 voiceNumber, f32 sampleRate) : _sampleRate(sampleRate) {
    _frequency = 440.f * std::pow(2.f, (voiceNumber - 69.f) / 12.f);
    for (i32 i=0; i<3; i++){
        setOscillatorType(oscillators::empty, i);
        setOscillatorAmplitude(1.0, i);
    }
}

void Voice::turnOn(){
    _adsr.reset();
    isPressed = true;
}

void Voice::turnOff(){
    isPressed = false;
}

void Voice::setAttack(f32 attack){
    if(attack < 0.0f || attack > 1.0f) {
		throw std::invalid_argument(fmt::format("Invalid attack value of: {}", attack));
    }
	_adsr.attackFactor = attack;
}

void Voice::setDecay(f32 decay){
	if(decay < 0.0f || decay > 1.0f) {
		throw std::invalid_argument(fmt::format("Invalid decay value of: {}", decay));
	}
	_adsr.decayFactor = decay;
}

void Voice::setSustain(f32 sustain){
	if(sustain < 0.0f || sustain > 1.0f) {
		throw std::invalid_argument(fmt::format("Invalid sustain value of: {}", sustain));
	}
	_adsr.sustainAmplitude = sustain;
}

void Voice::setRelease(f32 release){
	if(release < 0.0f || release > 1.0f) {
		throw std::invalid_argument(fmt::format("Invalid release value of: {}", release));
	}
	_adsr.releaseFactor = release;
}
}