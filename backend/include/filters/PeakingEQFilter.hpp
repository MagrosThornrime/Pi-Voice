#pragma once

#include "BwFilter.hpp"
#include <numbers>

namespace filters {
class PeakingEQFilter: public BwFilter {
public:
	void refresh() override;
};
}