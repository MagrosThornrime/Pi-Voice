#include <VoiceManager.hpp>

VoiceManager::VoiceManager(int voicesNumber){
    for(i32 i = 0; i < voicesNumber; i++){
        _voices.emplace_back(i);
    }
}

void VoiceManager::setAmplitude(f32 amplitude){
	for(auto& voice : _voices){
		voice.setAmplitude(amplitude);
	}
}

void VoiceManager::setOscillatorType(OscillatorType type){
	for(auto& voice : _voices){
		voice.setOscillatorType(type);
	}
}

f32 VoiceManager::getNextSample(){
	f32 sample = 0.0f;
	for(auto& voice : _voices){
		sample += voice.getNextSample();
	}
}

void VoiceManager::update(){
	for(auto& voice : _voices){
		voice.update();
	}
}

void VoiceManager::setActive(i32 voiceNumber, bool isActive){
	_voices[voiceNumber].isActive = isActive;
}