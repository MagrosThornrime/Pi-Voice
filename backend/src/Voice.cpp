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

Voice::Voice(i32 voiceNumber) : _voiceNumber(voiceNumber) {
    setOscillatorType(sine);
    _oscillator->setFrequency(440.0);
}