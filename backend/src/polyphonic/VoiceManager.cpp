#include <polyphonic/VoiceManager.hpp>
#include <string>

namespace polyphonic {
int VoiceManager::paCallbackFun(const void* input, void* output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags) {
	f32* out = (f32*)output;
	for (i32 i = 0; i < frameCount; i++) {
		update();
		f32 sample = _amplitude * _getNextSample();
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

void VoiceManager::setOscillatorType(oscillators::OscillatorType type, i32 index){
	auto lock = std::lock_guard(_oscillatorMutex);
	for(auto& voice : _voices){
		voice.setOscillatorType(type, index);
	}
}

void VoiceManager::setOscillatorAmplitude(f32 amplitude, i32 index){
	auto lock = std::lock_guard(_oscillatorMutex);
	for(auto& voice : _voices){
		voice.setOscillatorAmplitude(amplitude, index);
	}
}

void VoiceManager::setAmplitude(f32 amplitude){
	if(amplitude < 0.0 || amplitude > 1.0){
		throw std::runtime_error("Invalid amplitude value of: " + std::to_string(amplitude));
	}
	_amplitude = amplitude;
}

f32 VoiceManager::_getNextSample(){
	auto lock = std::lock_guard(_oscillatorMutex);
	f32 sample = 0.0f;
	for(auto& voice : _voices){
		sample += voice.getNextSample();
	}
	return sample;
}

void VoiceManager::update(){
	auto lock = std::lock_guard(_oscillatorMutex);
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

void VoiceManager::setAttack(f32 attack){
	for(auto& voice : _voices){
		voice.setAttack(attack);
	}
}

void VoiceManager::setDecay(f32 decay){
	for(auto& voice : _voices){
		voice.setDecay(decay);
	}
}

void VoiceManager::setSustain(f32 sustain){
	for(auto& voice : _voices){
		voice.setSustain(sustain);
	}
}

void VoiceManager::setRelease(f32 release){
	for(auto& voice : _voices){
		voice.setRelease(release);
	}
}
}