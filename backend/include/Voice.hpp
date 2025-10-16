#pragma once
#include <Oscillators.hpp>
#include <Types.hpp>
#include <memory>

class Voice {
    std::shared_ptr<Oscillator> _oscillator;
    i32 _voiceNumber;

public:
	bool isActive = false;

    explicit Voice(i32 voiceNumber);
    void setOscillatorType(OscillatorType oscillatorType);
    void setAmplitude(f32 amplitude);
    f32 getNextSample();
    void update();
	void setActive(i32 voiceNumber, bool isActive);
}