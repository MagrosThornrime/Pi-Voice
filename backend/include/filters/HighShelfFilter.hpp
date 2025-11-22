#pragma once

#include "BwFilter.hpp"
#include <numbers>

namespace filters {
class HighShelfFilter: public BwFilter {
public:
	void refresh() override;
};
}