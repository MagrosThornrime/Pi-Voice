#include "filters/LowShelfFilter.hpp"
#include <numbers>
#include <cmath>
#include <range/v3/view/concat.hpp>

LowShelfFilter::LowShelfFilter(const u32 order, const u32 channels, const f32 cutoffFrequency, const f32 samplingRate, const f32 quality, const f32 gainDB) {
	_prev = decltype(_prev)(channels);
	_channels = channels;

	constexpr auto pi = std::numbers::pi_v<f32>;

	if (order == 1) {
		const auto K = std::tan(pi * cutoffFrequency / samplingRate);
		const auto A = std::pow(10.f, gainDB / 40);
		const auto a0 = 1 + K;

		_b = {
			1 + A * K,
			1 - A * K
		};
		_a = {
			1 - K
		};

		for (auto&& a : _a) {
			a /= a0;
		}
		for (auto&& b : _b) {
			b /= a0;
		}
	} else {
		const auto omega = (2 * pi) * cutoffFrequency / samplingRate;
		const auto cosOmega = std::cos(omega);
		const auto sinOmega = std::sin(omega);
		const auto alpha = sinOmega / (2 * quality);
		const auto A = std::pow(10.f, gainDB / 40);
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