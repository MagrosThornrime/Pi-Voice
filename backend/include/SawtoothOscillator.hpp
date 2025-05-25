#pragma once

class SawtoothOscillator {
    float _minPhase, _maxPhase, _phaseIncrement;
    float _currentPhase = 0.0f;

public:
    SawtoothOscillator(float minPhase, float maxPhase, float phaseIncrement)
    : _minPhase(minPhase), _maxPhase(maxPhase), _phaseIncrement(phaseIncrement) {};
    float getNextPhase();
};