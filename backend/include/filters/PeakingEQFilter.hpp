#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class PeakingEQFilter: public BiquadFilter {
public:
	using BwFilter::BwFilter;
	void refresh() override;
};
}