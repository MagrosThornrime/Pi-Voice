#pragma once
#include <Oscillators.hpp>
#include <Types.hpp>
#include <memory>
#include <ADSR.hpp>

class Voice {
    std::shared_ptr<oscillators::Oscillator> _oscillator;
	f32 _frequency;
	f32 _sampleRate;

    ADSR _adsr;

public:
	bool isActive = false;

    Voice(i32 voiceNumber, f32 sampleRate);
    void setOscillatorType(oscillators::OscillatorType oscillatorType);
    f32 getNextSample();
    void update();

	void turnOn();
    void turnOff();

	void setADSRAttack(f32 attack);
	void setADSRDecay(f32 decay);
	void setADSRSustain(f32 sustain);
	void setADSRRelease(f32 release);
};