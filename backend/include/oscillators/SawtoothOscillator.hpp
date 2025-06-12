#pragma once
#include <oscillators/Oscillator.hpp>

class SawtoothOscillator : public Oscillator {
    float _minPhase, _maxPhase, _phaseIncrement;

public:
    SawtoothOscillator(float minPhase, float maxPhase, float phaseIncrement)
    : _minPhase(minPhase), _maxPhase(maxPhase), _phaseIncrement(phaseIncrement) {};
    float getNextPhase() override;
};