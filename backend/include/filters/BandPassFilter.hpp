#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class BandPassFilter: public BiquadFilter {
public:
	void refresh() override;
};
}