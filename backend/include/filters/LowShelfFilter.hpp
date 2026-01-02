#pragma once

#include "BwFilter.hpp"
#include <numbers>

namespace filters {
class LowShelfFilter: public BwFilter {
public:
	using BwFilter::BwFilter;
	void refresh() override;
};
}