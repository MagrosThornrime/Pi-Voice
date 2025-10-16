#include <Voice.hpp>

Voice::setOscillatorType(OscillatorType oscillatorType) {
    switch (oscillatorType) {
        case noise:
            _oscillator = std::make_shared<Noise>();
            break;
        case sine:
            _oscillator = std::make_shared<Sine>();
            break;
        case sawtooth:
            _oscillator = std::make_shared<Sawtooth>();
            break;
        case square:
            _oscillator = std::make_shared<Square>();
            break;
        case triangle:
            _oscillator = std::make_shared<Triangle>();
            break;
    }
}

void Voice::setAmplitude(amplitude){
	_oscillator->setAmplitude(amplitude);
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

Voice::Voice(i32 voiceNumber) : _voiceNumber(voiceNumber) {
    setOscillatorType(sine);
	f32 frequency = 440.f * std::pow(2.f, (voiceNumber - 69.f) / 12.f;
    _oscillator->setFrequency(frequency);
}