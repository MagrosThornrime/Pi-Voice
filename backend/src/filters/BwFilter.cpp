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
	auto in = (const float*)inputBuffer;
	auto out = (float*)outputBuffer;


	u32 idx = 0;
	for (u32 i = 0; i < numFrames; ++i) {
		for (u32 ch = 0; ch < _channels; ++ch, ++idx) {
			auto&& prev = _prev[ch];

			float y = _b[0] * in[idx] + _b[1] * prev.x1 + _b[2] * prev.x2
				- _a[0] * prev.y1 - _a[1] * prev.y2;

			prev.x2 = prev.x1;
			prev.x1 = in[idx];

			prev.y2 = prev.y1;
			prev.y1 = y;

			out[idx] = y;
		}
	}

	return paContinue;
}

void BwFilter::_set(const u32 channels, const std::array<float, 2>& a, const std::array<float, 3>& b) {
	_channels = channels;
	_a = a;
	_b = b;
	_prev = decltype(_prev)(channels);
}