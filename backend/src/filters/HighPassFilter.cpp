#include "filters/HighPassFilter.hpp"
#include <numbers>
#include <cmath>
#include <range/v3/view/concat.hpp>

namespace filters {
void HighPassFilter::refresh() {
	constexpr auto pi = std::numbers::pi_v<f32>;

	if (_order == 1) {
		const auto alpha = std::tan(pi * _cutoff / _sampleRate);
		const auto a0 = 1 + alpha;

		_b = {
			1,
			-1,
			0
		};
		_a = {
			alpha - 1,
			0
		};

		for (auto&& a : _a) {
			a /= a0;
		}
		for (auto&& b : _b) {
			b /= a0;
		}
	} else {
		const auto omega = (2 * pi) * _cutoff / _sampleRate;
		const auto cosOmega = std::cos(omega);
		const auto sinOmega = std::sin(omega);
		const auto alpha = sinOmega / (2 * _quality);

		const auto a0 = 1 + alpha;

		_b = {
			(1 + cosOmega) / 2,
			-1 - cosOmega,
			(1 + cosOmega) / 2
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
}