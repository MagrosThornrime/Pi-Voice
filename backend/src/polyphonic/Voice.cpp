#include <polyphonic/Voice.hpp>


void Voice::setOscillatorType(oscillators::OscillatorType oscillatorType, i32 index) {
    if(index < 0 || index > 2){
        return;
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
        return;
    }
    if(amplitude < 0.0f || amplitude > 1.0f){
        return;
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