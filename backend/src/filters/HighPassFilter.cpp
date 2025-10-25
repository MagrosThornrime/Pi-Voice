#include "filters/HighPassFilter.hpp"
#include <numbers>
#include <cmath>
#include <range/v3/view/concat.hpp>

HighPassFilter::HighPassFilter(const u32 order, const u32 channels, const f32 cutoffFrequency, const f32 samplingRate, const f32 quality) {
	_prev = decltype(_prev)(channels);
	_channels = channels;

	constexpr auto pi = std::numbers::pi_v<f32>;

	if (order == 1) {
		const auto K = std::tan(pi * cutoffFrequency / samplingRate);
		const auto a0 = 1 + K;

		_b = {
			1,
			-1
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