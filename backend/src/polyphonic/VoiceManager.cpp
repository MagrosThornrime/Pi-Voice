#include <polyphonic/VoiceManager.hpp>
#include <fmt/core.h>

namespace polyphonic {

void VoiceManager::generateSound(std::vector<f32>& out, u32 frames)
{
	f32* ptr = out.data();
	for (i32 i = 0; i < frames; i++) {
		update();
		f32 sample = _getNextSample();
		for (i32 ch = 0; ch < _channels; ch++) {
			*ptr++ = sample;
		}
	}
}


VoiceManager::VoiceManager(i32 voicesNumber, f32 sampleRate, u32 channels,
	std::shared_ptr<fileio::SampleManager> sampleManager): _channels(channels) {
	for (i32 i = 0; i < voicesNumber; i++) {
		_voices.emplace_back(sampleRate, sampleManager);
	}
}

void VoiceManager::setOscillatorType(const std::string& type, i32 index) {
	if (index < 0 || index > 2) {
		throw std::invalid_argument(fmt::format("Invalid index value of: {}", index));
	}
	auto lock = std::lock_guard(_oscillatorMutex);
	_oscillatorTypes[index] = type;
}

f32 VoiceManager::_getNextSample() {
	auto lock = std::lock_guard(_oscillatorMutex);
	f32 sample = 0.0f;
	for (auto& voice : _voices) {
		if (!voice.isPlaying()) {
			continue;
		}
		sample += voice.getNextSample();
	}
	return sample / _voices.size();
}

void VoiceManager::update() {
	auto lock = std::lock_guard(_oscillatorMutex);
	for (auto& voice : _voices) {
		voice.update();
	}
}

i32 VoiceManager::_findVoice() {
	for (i32 i = 0; i < _voices.size(); i++) {
		if (!_voices[i].isPlaying()) {
			return i;
		}
	}
	return -1;
}

void VoiceManager::turnOn(i32 voiceNumber) {
	auto lock = std::lock_guard(_oscillatorMutex);
	i32 index = _findVoice();
	if (index == -1) {
		return;
	}
	_voices[index].setNote(voiceNumber);
	for (i32 i = 0; i < 3; i++) {
		_voices[index].setOscillatorType(_oscillatorTypes[i], i);
	}
	_voices[index].setAttack(_attack);
	_voices[index].setDecay(_decay);
	_voices[index].setSustain(_sustain);
	_voices[index].setRelease(_release);
	_voices[index].turnOn();
}

void VoiceManager::turnOff(i32 voiceNumber) {
	auto lock = std::lock_guard(_oscillatorMutex);
	for (auto& voice : _voices) {
		if (voice.getNote() == voiceNumber && voice.isPlaying()) {
			voice.turnOff();
		}
	}
}

void VoiceManager::setAttack(f32 attack) {
	if (attack < 0.0f || attack > 1.0f) {
		throw std::invalid_argument(fmt::format("Invalid attack value of: {}", attack));
	}
	auto lock = std::lock_guard(_oscillatorMutex);
	_attack = attack;
}

void VoiceManager::setDecay(f32 decay) {
	if (decay < 0.0f || decay > 1.0f) {
		throw std::invalid_argument(fmt::format("Invalid decay value of: {}", decay));
	}
	auto lock = std::lock_guard(_oscillatorMutex);
	_decay = decay;
}

void VoiceManager::setSustain(f32 sustain) {
	if (sustain < 0.0f || sustain > 1.0f) {
		throw std::invalid_argument(fmt::format("Invalid sustain value of: {}", sustain));
	}
	auto lock = std::lock_guard(_oscillatorMutex);
	_sustain = sustain;
}

void VoiceManager::setRelease(f32 release) {
	if (release < 0.0f || release > 1.0f) {
		throw std::invalid_argument(fmt::format("Invalid release value of: {}", release));
	}
	auto lock = std::lock_guard(_oscillatorMutex);
	_release = release;
}

bool VoiceManager::hasActiveVoices() {
	for (auto& voice : _voices) {
		if (voice.isPlaying()) {
			return true;
		}
	}
	return false;
}

}