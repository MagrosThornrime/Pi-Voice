#include "filters/LowShelfFilter.hpp"
#include <numbers>
#include <cmath>
#include <range/v3/view/concat.hpp>

namespace filters {
void LowShelfFilter::refresh() {
	constexpr auto pi = std::numbers::pi_v<f32>;

	if (_order == 1) {
		const auto alpha = std::tan(pi * _cutoff / _sampleRate);
		const auto A = std::pow(10.f, _gainDB / 20);
		const auto sqrtA = std::sqrt(A);
		const auto a0 = 1 + alpha;

		_b = {
			1 + sqrtA * alpha,
			1 - sqrtA * alpha,
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
		const auto A = std::pow(10.f, _gainDB / 40);
		const auto beta = 2 * std::sqrt(A) * alpha;

		const auto a0 = A + 1 + (A - 1) * cosOmega + beta;

		_b = {
			A * (A + 1 - (A - 1) * cosOmega + beta),
			2 * A * (A - 1 - (A + 1) * cosOmega),
			A * (A + 1 - (A - 1) * cosOmega - beta)
		};
		_a = {
			-2 * (A - 1 + (A + 1) * cosOmega),
			A + 1 + (A - 1) * cosOmega - beta
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