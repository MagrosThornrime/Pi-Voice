#include <polyphonic/Voice.hpp>
#include <fmt/core.h>

namespace polyphonic {
void Voice::setOscillatorType(const std::string& oscillatorType, i32 index) {
    if(index < 0 || index > 2){
        throw std::invalid_argument(fmt::format("Invalid index value of: {}", index));
    }
    if (oscillatorType == "empty") {
        _oscillators[index] = std::make_shared<oscillators::Oscillator>(_sampleRate, _voiceNumber);
    }
    else if (oscillatorType == "sine") {
        _oscillators[index] = std::make_shared<oscillators::SineOscillator>(_sampleRate, _voiceNumber);
    }
    else if (oscillatorType == "saw") {
        _oscillators[index] = std::make_shared<oscillators::SawtoothOscillator>(_sampleRate, _voiceNumber);
    }
    else if (oscillatorType == "square") {
        _oscillators[index] = std::make_shared<oscillators::SquareOscillator>(_sampleRate, _voiceNumber);
    }
    else if (oscillatorType == "triangle") {
        _oscillators[index] = std::make_shared<oscillators::TriangleOscillator>(_sampleRate, _voiceNumber);
    }
    else {
        const std::vector<f32>& sample = _sampleManager->getSample(oscillatorType);
        _oscillators[index] = std::make_shared<oscillators::ModulatedOscillator>(_sampleRate, _voiceNumber, sample);
    }
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

Voice::Voice(f32 sampleRate, std::shared_ptr<fileio::SampleManager> sampleManager)
: _sampleRate(sampleRate), _sampleManager(sampleManager) {
    for (i32 i=0; i<3; i++){
        setOscillatorType("empty", i);
        setOscillatorAmplitude(1.0, i);
    }
}

void Voice::setNote(i32 voiceNumber){
    _voiceNumber = voiceNumber;
    for (i32 i=0; i<3; i++){
        _oscillators[i]->setNote(voiceNumber);
    }
}

i32 Voice::getNote(){
    return _voiceNumber;
}

void Voice::turnOn(){
    for(i32 i=0; i<3; i++){
        _oscillators[i]->reset();
    }
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

bool Voice::isPlaying(){
    return _adsr.isPlaying();
}
}