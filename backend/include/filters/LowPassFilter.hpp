#pragma once

#include "Types.hpp"
#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class LowPassFilter: public BiquadFilter {
public:
	using BwFilter::BwFilter;
	void refresh() override;
};
}