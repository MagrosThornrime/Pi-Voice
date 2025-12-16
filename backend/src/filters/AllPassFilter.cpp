#include "filters/AllPassFilter.hpp"
#include <numbers>
#include <cmath>
#include <range/v3/view/concat.hpp>

namespace filters {
void AllPassFilter::refresh() {
	_prev.resize(_channels);
	for (auto&& v : _prev) {
		v = {};
	}

	constexpr auto pi = std::numbers::pi_v<f32>;

	const auto omega = (2 * pi) * _cutoff / _sampleRate;
	const auto cosOmega = std::cos(omega);
	const auto sinOmega = std::sin(omega);
	const auto alpha = sinOmega / (2 * _quality);

	const auto a0 = 1 + alpha;

	_b = {
		1 - alpha,
		-2 * cosOmega,
		1 + alpha
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