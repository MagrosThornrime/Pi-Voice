#include "filters/PeakingEQFilter.hpp"
#include <numbers>
#include <cmath>
#include <range/v3/view/concat.hpp>

namespace filters {
void PeakingEQFilter::refresh() {
	_prev.resize(_channels);
	for (auto&& v : _prev) {
		v = {};
	}

	const auto omega = (2 * std::numbers::pi_v<f32>) * _cutoff / _sampleRate;
	const auto cosOmega = std::cos(omega);
	const auto sinOmega = std::sin(omega);
	const auto alpha = sinOmega / (2 * _quality);
	const auto A = std::pow(10.f, _gainDB / 40);

	const auto a0 = 1 + alpha / A;

	_b = {
		1 + alpha * A,
		-2 * cosOmega,
		1 - alpha * A
	};
	_a = {
		-2 * cosOmega,
		1 - alpha / A
	};

	for (auto&& a : _a) {
		a /= a0;
	}
	for (auto&& b : _b) {
		b /= a0;
	}
}
}