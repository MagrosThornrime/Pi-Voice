#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class BandPassFilter: public BiquadFilter {
public:
	using BwFilter::BwFilter;
	void refresh() override;
};
}