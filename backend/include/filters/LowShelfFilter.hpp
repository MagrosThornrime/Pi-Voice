#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class LowShelfFilter: public BiquadFilter {
public:
	void refresh() override;
};
}