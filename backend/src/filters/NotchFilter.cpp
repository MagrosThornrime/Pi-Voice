#include "filters/NotchFilter.hpp"
#include <numbers>
#include <cmath>
#include <range/v3/view/concat.hpp>

namespace filters {
void NotchFilter::refresh() {
	if (_prev.empty()) {
		_prev = decltype(_prev)(_channels);
	}

	const auto omega = (2 * std::numbers::pi_v<f32>) * _cutoff / _sampleRate;
	const auto cosOmega = std::cos(omega);
	const auto sinOmega = std::sin(omega);
	const auto alpha = sinOmega / (2 * _quality);

	const auto a0 = 1 + alpha;

	_b = {
		1,
		-2 * cosOmega,
		1
	};
	_a = {
		-2 * cosOmega,
		1 - alpha
	};

	for (auto&& a : _a) {
		a /= a0;
	}
	for (auto&& b : _b) {
		b /= a0;
	}
}
}