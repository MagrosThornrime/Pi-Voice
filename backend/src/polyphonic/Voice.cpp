#include <polyphonic/Voice.hpp>
#include <fmt/core.h>

namespace polyphonic {
void Voice::setOscillatorType(const std::string& oscillatorType, i32 index) {
    if (oscillatorType == "empty") {
        _oscillators[index] = std::make_shared<oscillators::Oscillator>(_sampleRate, _voiceNumber);
    }
    else if (oscillatorType == "sine") {
        _oscillators[index] = std::make_shared<oscillators::SineOscillator>(_sampleRate, _voiceNumber);
    }
    else if (oscillatorType == "sawtooth") {
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
        sample += _oscillators[i]->getNextSample();
    }
	return _adsr.getAmplitude(isPressed) * sample;
}

void Voice::update(){
    for (i32 i=0; i<3; i++){
        _oscillators[i]->advance();
    }
}

Voice::Voice(f32 sampleRate, std::shared_ptr<fileio::SampleManager> sampleManager)
: _sampleRate(sampleRate), _sampleManager(sampleManager) {
    for (i32 i=0; i<3; i++){
        setOscillatorType("empty", i);
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
	_adsr.attackFactor = attack;
}

void Voice::setDecay(f32 decay){
	_adsr.decayFactor = decay;
}

void Voice::setSustain(f32 sustain){
	_adsr.sustainAmplitude = sustain;
}

void Voice::setRelease(f32 release){
	_adsr.releaseFactor = release;
}

bool Voice::isPlaying(){
    return _adsr.isPlaying();
}
}