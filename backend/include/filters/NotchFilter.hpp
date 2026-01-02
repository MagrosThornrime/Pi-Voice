#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class NotchFilter: public BiquadFilter {
public:
	using BwFilter::BwFilter;
	void refresh() override;
};
}