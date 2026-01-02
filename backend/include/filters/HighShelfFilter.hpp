#pragma once

#include "BwFilter.hpp"
#include <numbers>

namespace filters {
class HighShelfFilter: public BwFilter {
public:
	using BwFilter::BwFilter;
	void refresh() override;
};
}