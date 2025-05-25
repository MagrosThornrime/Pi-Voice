#include <SawtoothOscillator.h>

float SawtoothOscillator::getNextPhase() const {
    float current = _currentPhase;
    _currentPhase += _phaseIncrement;
    if (_currentPhase > _maxPhase) {
        _currentPhase = _minPhase;
    }
    return current;
}