#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class LowShelfFilter: public BiquadFilter {
public:
	using BiquadFilter::BiquadFilter;
	void refresh() override;
};
}