#pragma once

#include "BwFilter.hpp"
#include <numbers>

namespace filters {
class NotchFilter: public BwFilter {
public:
	NotchFilter(const u32 channels, const f32 cutoffFrequency, const f32 samplingRate, const f32 quality = std::numbers::sqrt2_v<f32> / 2);
};
}