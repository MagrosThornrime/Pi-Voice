#include <effects/AllPassStage.hpp>

namespace effects {

void AllPassStage::setCoefficient(f32 controlFreq){
    _coeff = (1.0f - controlFreq) / (1.0f + controlFreq);
}

f32 AllPassStage::processSample(f32 input){
    f32 y = -_coeff * input + _buffer;
    _buffer = input + _coeff * y;
    return y;
}

}