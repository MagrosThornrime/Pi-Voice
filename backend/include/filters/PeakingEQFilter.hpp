#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class PeakingEQFilter: public BiquadFilter {
public:
	using BiquadFilter::BiquadFilter;
	void refresh() override;
};
}