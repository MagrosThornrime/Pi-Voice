#pragma once

#include "BwFilter.hpp"
#include <numbers>

namespace filters {
class HighPassFilter: public BwFilter {
public:
	using BwFilter::BwFilter;
	void refresh() override;
};
}