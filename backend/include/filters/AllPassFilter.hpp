#pragma once

#include "BwFilter.hpp"
#include <numbers>

namespace filters {
class AllPassFilter: public BwFilter {
public:
	void refresh() override;
};
}