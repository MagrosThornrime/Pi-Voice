#pragma once
#include <Types.hpp>

namespace effects {
class AllPassStage {
    f32 _coeff = 0.0f;
    f32 _buffer = 0.0f;

public:
    AllPassStage() = default;

    void setCoefficient(f32 controlFreq);

    f32 processSample(f32 input);
};
}