#include <VoiceManager.hpp>

i32 VoiceManager::paCallback(const void* input, void* output,
	u64 frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags) {
	f32* out = (f32*)output;
	for (i32 i = 0; i < frameCount; i++) {
		update();
		f32 sample = amplitude * _getNextSample();
		*out++ = sample;
		*out++ = sample;
	}
	return 0;
}

VoiceManager::VoiceManager(i32 voicesNumber, f32 sampleRate){
    for(i32 i = 0; i < voicesNumber; i++){
        _voices.emplace_back(i, sampleRate);
    }
}

void VoiceManager::setOscillatorType(oscillators::OscillatorType type){
	for(auto& voice : _voices){
		voice.setOscillatorType(type);
	}
}

f32 VoiceManager::_getNextSample(){
	f32 sample = 0.0f;
	for(auto& voice : _voices){
		sample += voice.getNextSample();
	}
	return sample;
}

void VoiceManager::update(){
	for(auto& voice : _voices){
		voice.update();
	}
}

void VoiceManager::turnOn(i32 voiceNumber){
	_voices[voiceNumber].turnOn();
}

void VoiceManager::turnOff(i32 voiceNumber){
    _voices[voiceNumber].turnOff();
}

void VoiceManager::setADSRAttack(i32 attack){
	for(auto& voice : _voices){
		voice.setADSRAttack(attack);
	}
}

void VoiceManager::setADSRDecay(i32 decay){
	for(auto& voice : _voices){
		voice.setADSRDecay(decay);
	}
}

void VoiceManager::setADSRSustain(f32 sustain){
	for(auto& voice : _voices){
		voice.setADSRSustain(sustain);
	}
}

void VoiceManager::setADSRRelease(i32 release){
	for(auto& voice : _voices){
		voice.setADSRRelease(release);
	}
}