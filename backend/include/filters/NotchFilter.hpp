#pragma once

#include "BwFilter.hpp"
#include <numbers>

namespace filters {
class NotchFilter: public BwFilter {
public:
	void refresh() override;
};
}