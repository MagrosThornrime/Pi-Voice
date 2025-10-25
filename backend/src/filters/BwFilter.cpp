#include "filters/BwFilter.hpp"
#include <span>
#include <range/v3/view/zip.hpp>
#include <range/v3/view/chunk.hpp>
#include <fmt/core.h>

BwFilter::BwFilter(const u32 channels, const std::array<float, 2>& a, const std::array<float, 3>& b) {
	_set(channels, a, b);
}

int BwFilter::paCallbackFun(const void* inputBuffer, void* outputBuffer, unsigned long numFrames,
	const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags) {
// Treat nullptr outputBuffer as required (PortAudio contract usually requires it non-null).
	if (!outputBuffer) return paAbort;

	const float* in = static_cast<const float*>(inputBuffer);
	float* out = static_cast<float*>(outputBuffer);

	// Snapshot important state to local variables (avoid torn reads)
	const u32 channels = _channels;
	std::array<float, 3> b = _b; // copy
	std::array<float, 2> a = _a; // copy
	auto prevData = &_prev[0];   // pointer to vector data (assumes vector sized already)
	// NOTE: you may prefer to copy _prev into a local array if you plan to modify coefficients concurrently.
	// But we won't reassign _prev here, only read/modify its Prev elements.

	// If inputBuffer is null, treat input samples as zeros (avoid reading null)
	const bool haveInput = (in != nullptr);

	u32 idx = 0;

	// tiny offset to avoid denormals (very small)
	constexpr float denormalGuard = 1e-18f;

	for (u32 i = 0; i < numFrames; ++i) {
		for (u32 ch = 0; ch < channels; ++ch, ++idx) {
			BwFilter::Prev& prev = prevData[ch];

			const float x_n = haveInput ? in[idx] : 0.0f;

			// difference equation
			float y = b[0] * x_n + b[1] * prev.x1 + b[2] * prev.x2 - a[0] * prev.y1 - a[1] * prev.y2;

			// denormal protection: if result extremely small, force to zero-ish
			if (std::abs(y) < 1e-25f) y += denormalGuard;

			// detect bad values and reset state if needed
			if (!std::isfinite(y)) {
				// reset this channel's history to silence to recover
				prev.x1 = prev.x2 = prev.y1 = prev.y2 = 0.0f;
				y = 0.0f;
			} else {
				// advance states
				prev.x2 = prev.x1;
				prev.x1 = x_n;

				prev.y2 = prev.y1;
				prev.y1 = y;
			}

			out[idx] = y;
		}
	}

	if (statusFlags != 0)
		fmt::println("{}", statusFlags);

	return paContinue;
}

void BwFilter::_set(const u32 channels, const std::array<float, 2>& a, const std::array<float, 3>& b) {
	_channels = channels;
	_a = a;
	_b = b;
	_prev = decltype(_prev)(channels);
}