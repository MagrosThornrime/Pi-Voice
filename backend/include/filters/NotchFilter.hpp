#pragma once

#include "BwFilter.hpp"
#include <numbers>

namespace filters {
class NotchFilter: public BwFilter {
public:
	using BwFilter::BwFilter;
	void refresh() override;
};
}