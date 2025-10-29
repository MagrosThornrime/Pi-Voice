#pragma once

#include "BwFilter.hpp"
#include <numbers>

class HighShelfFilter: public BwFilter {
public:
	HighShelfFilter(const u32 order, const u32 channels, const f32 cutoffFrequency, const f32 samplingRate, const f32 quality = std::numbers::sqrt2_v<f32> / 2, const f32 gainDB = 0);
};