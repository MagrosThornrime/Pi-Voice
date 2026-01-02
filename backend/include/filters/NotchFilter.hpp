#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class NotchFilter: public BiquadFilter {
public:
	using BiquadFilter::BiquadFilter;
	void refresh() override;
};
}