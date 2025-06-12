#include <oscillators/SawtoothOscillator.hpp>

float SawtoothOscillator::getNextPhase() {
    float current = _currentPhase;
    _currentPhase += _phaseIncrement;
    if (_currentPhase > _maxPhase) {
        _currentPhase = _minPhase;
    }
    return current;
}