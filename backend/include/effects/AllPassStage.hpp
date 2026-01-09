#pragma once
#include <Types.hpp>

namespace effects {

/// @param Simplier implementation of the AllPass filter, used in various effects
/// @param Shifts the phase relationship between different frequencies
class AllPassStage {

    f32 _coeff = 0.0f;
    f32 _buffer = 0.0f;

public:
    /// @param Constructor
    AllPassStage() = default;

    /// @brief Calculates the _coeff value
    /// @param controlFreq normalized control frequency
    /// (depends on the frequency you want to be most affected by the phase shift)
    void setCoefficient(f32 controlFreq);

    /// @brief Applies the filter to the signal
    /// @param input input sample
    f32 processSample(f32 input);
};
}