#pragma once

#include "Types.hpp"
#include "BwFilter.hpp"
#include <numbers>

namespace filters {
class LowPassFilter: public BwFilter {
public:
	void refresh() override;
};
}