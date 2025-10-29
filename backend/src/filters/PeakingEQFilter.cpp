#include "filters/PeakingEQFilter.hpp"
#include <numbers>
#include <cmath>
#include <range/v3/view/concat.hpp>

PeakingEQFilter::PeakingEQFilter(const u32 channels, const f32 cutoffFrequency, const f32 samplingRate, const f32 quality, const f32 gainDB) {
	_prev = decltype(_prev)(channels);
	_channels = channels;

	const auto omega = (2 * std::numbers::pi_v<f32>) * cutoffFrequency / samplingRate;
	const auto cosOmega = std::cos(omega);
	const auto sinOmega = std::sin(omega);
	const auto alpha = sinOmega / (2 * quality);
	const auto A = std::pow(10.f, gainDB / 40);

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