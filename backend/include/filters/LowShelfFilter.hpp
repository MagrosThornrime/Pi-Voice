#pragma once

#include "BwFilter.hpp"
#include <numbers>

namespace filters {
class LowShelfFilter: public BwFilter {
public:
	void refresh() override;
};
}