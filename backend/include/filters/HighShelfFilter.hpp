#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class HighShelfFilter: public BiquadFilter {
public:
	void refresh() override;
};
}