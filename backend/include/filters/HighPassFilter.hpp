#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class HighPassFilter: public BiquadFilter {
public:
	using BiquadFilter::BiquadFilter;
	void refresh() override;
};
}