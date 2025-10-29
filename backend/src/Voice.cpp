#include <Voice.hpp>


void Voice::setOscillatorType(oscillators::OscillatorType oscillatorType) {
    switch (oscillatorType) {
        case oscillators::noise:
            _oscillator = std::make_shared<oscillators::NoiseOscillator>(_sampleRate);
            break;
        case oscillators::sine:
            _oscillator = std::make_shared<oscillators::SineOscillator>(_sampleRate);
            break;
        case oscillators::sawtooth:
            _oscillator = std::make_shared<oscillators::SawtoothOscillator>(_sampleRate);
            break;
        case oscillators::square:
            _oscillator = std::make_shared<oscillators::SquareOscillator>(_sampleRate);
            break;
        case oscillators::triangle:
            _oscillator = std::make_shared<oscillators::TriangleOscillator>(_sampleRate);
            break;
    }
	_oscillator->setFrequency(_frequency);
}

f32 Voice::getNextSample(){
	return _adsr.getAmplitude(isActive) * _oscillator->getNextSample();
}

void Voice::update(){
	_oscillator->advance();
}

Voice::Voice(i32 voiceNumber, f32 sampleRate) : _sampleRate(sampleRate) {
    setOscillatorType(oscillators::triangle);
	_frequency = 440.f * std::pow(2.f, (voiceNumber - 69.f) / 12.f);
    _oscillator->setFrequency(_frequency);
}

void Voice::turnOn(){
    _adsr.reset();
    isActive = true;
}

void Voice::turnOff(){
    isActive = false;
}

void Voice::setAttack(f32 attack){
    if(attack > 0.0f && attack < 1.0f) {
        _adsr.attackFactor = attack;
    }
}

void Voice::setDecay(f32 decay){
    if(decay > 0.0f && decay < 1.0f) {
        _adsr.decayFactor = decay;
    }
}

void Voice::setSustain(f32 sustain){
    if(sustain > 0.0f && sustain < 1.0f) {
        _adsr.sustainAmplitude = sustain;
    }
}

void Voice::setRelease(f32 release){
    if(release > 0.0f && release < 1.0f) {
        _adsr.releaseFactor = release;
    }
}