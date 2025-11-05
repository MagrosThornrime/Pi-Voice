#include <polyphonic/ADSR.hpp>
#include <iostream>

namespace polyphonic {
void ADSR::reset(){
	_amplitude = 0.0f;
	_stage = attack;
}

f32 ADSR::getAmplitude(bool isPressed) {
    if (isPressed) {
        if (_stage == attack) {
            _amplitude += attackFactor;
            if (_amplitude >= 1.0f) { 
				_amplitude = 1.0f;
				_stage = decay;
			}
        }
        if (_stage == decay) {
            _amplitude -= decayFactor;
        	if (_amplitude <= sustainAmplitude) {
 				_amplitude = sustainAmplitude;
				_stage = sustain;
			}
        }
    }
	else {
	    _stage = release;
	    _amplitude -= releaseFactor;
	    if (_amplitude <= 0.0f){
			_amplitude = 0.0f;
		}
	}
    return _amplitude;
}
}