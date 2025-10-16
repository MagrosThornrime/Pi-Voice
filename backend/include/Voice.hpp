#pragma once
#include <Oscillators.hpp>
#include <Types.hpp>
#include <memory>

class Voice {
    std::shared_ptr<oscillators::Oscillator> _oscillator;
    i32 _voiceNumber;
	f32 _sampleRate;

public:
	bool isActive = false;

    Voice(i32 voiceNumber, f32 sampleRate);
    void setOscillatorType(oscillators::OscillatorType oscillatorType);
    f32 getNextSample();
    void update();
	void setActive(i32 voiceNumber, bool isActive);
};