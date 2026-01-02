#pragma once

#include "BiquadFilter.hpp"
#include <numbers>

namespace filters {
class NotchFilter: public BiquadFilter {
public:
	void refresh() override;
};
}