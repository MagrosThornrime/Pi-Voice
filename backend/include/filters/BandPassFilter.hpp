#pragma once

#include "BwFilter.hpp"
#include <numbers>

namespace filters {
class BandPassFilter: public BwFilter {
public:
	void refresh() override;
};
}