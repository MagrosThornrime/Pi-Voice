#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class AllPassFilter: public BiquadFilter {
public:
	using BiquadFilter::BiquadFilter;
	void refresh() override;
};
}