#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class HighShelfFilter: public BiquadFilter {
public:
	using BwFilter::BwFilter;
	void refresh() override;
};
}