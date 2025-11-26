#pragma once

#include <Types.hpp>

namespace filters {

struct FilterType {
	enum Value: u32 {
		allPass,
		bandPass,
		highPass,
		lowPass,
		highShelf,
		lowShelf,
		notch,
		peakingEQ
	};
};

}