#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class HighPassFilter: public BiquadFilter {
public:
	void refresh() override;
};
}