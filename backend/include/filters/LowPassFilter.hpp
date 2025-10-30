#pragma once

#include "Types.hpp"
#include "BwFilter.hpp"
#include <numbers>

class LowPassFilter: public BwFilter {
public:
	LowPassFilter(const u32 order, const u32 channels, const f32 cutoffFrequency, const f32 samplingRate, const f32 quality = std::numbers::sqrt2_v<f32> / 2);
};