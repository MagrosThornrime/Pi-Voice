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
}

f32 Voice::getNextSample(){
	if(isActive){
		return _oscillator->getNextSample();
	}
	return 0.0f;
}

void Voice::update(){
	if(isActive){
		_oscillator->advance();
	}
}

Voice::Voice(i32 voiceNumber, f32 sampleRate) : _voiceNumber(voiceNumber), _sampleRate(sampleRate) {
    setOscillatorType(oscillators::sine);
	f32 frequency = 440.f * std::pow(2.f, (voiceNumber - 69.f) / 12.f);
    _oscillator->setFrequency(frequency);
}