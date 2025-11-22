#pragma once

#include "BwFilter.hpp"
#include <numbers>

namespace filters {
class HighPassFilter: public BwFilter {
public:
	void refresh() override;
};
}