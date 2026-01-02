#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class AllPassFilter: public BiquadFilter {
public:
	void refresh() override;
};
}