#include <filters/BwFilter.hpp>
#include <filters/FilterParams.hpp>
#include <span>
#include <range/v3/view/zip.hpp>
#include <range/v3/view/chunk.hpp>
#include <fmt/core.h>

#include <filters/AllPassFilter.hpp>
#include <filters/BandPassFilter.hpp>
#include <filters/HighPassFilter.hpp>
#include <filters/HighShelfFilter.hpp>
#include <filters/LowPassFilter.hpp>
#include <filters/LowShelfFilter.hpp>
#include <filters/NotchFilter.hpp>
#include <filters/PeakingEQFilter.hpp>

namespace filters {

BwFilter::BwFilter(const u32 channels, const std::array<float, 2>& a, const std::array<float, 3>& b) {
	_set(channels, a, b);
}

std::shared_ptr<BwFilter> BwFilter::create(FilterType::Value filter) {
	std::shared_ptr<BwFilter> result = nullptr;
	switch (filter) {
		case FilterType::allPass: result = std::make_shared<AllPassFilter>();
		case FilterType::bandPass: result = std::make_shared<BandPassFilter>();
		case FilterType::highPass: result = std::make_shared<HighPassFilter>();
		case FilterType::highShelf: result = std::make_shared<HighShelfFilter>();
		case FilterType::lowPass: result = std::make_shared<LowPassFilter>();
		case FilterType::lowShelf: result = std::make_shared<LowShelfFilter>();
		case FilterType::notch: result = std::make_shared<NotchFilter>();
		case FilterType::peakingEQ: result = std::make_shared<PeakingEQFilter>();
	}
	if (result) {
		result->refresh();
	}

	return result;
}

void BwFilter::processSound(std::vector<f32>& inputBuffer,
	std::vector<f32>& outputBuffer,
	u32 frames) {
	const float* in = inputBuffer.data();
	float* out = outputBuffer.data();

	u32 idx = 0;
	for (u32 i = 0; i < frames; ++i) {
		for (u32 ch = 0; ch < _channels; ++ch, ++idx) {

			auto& prev = _prev[ch];

			float y =
				_b[0] * in[idx] +
				_b[1] * prev.x1 +
				_b[2] * prev.x2 -
				_a[0] * prev.y1 -
				_a[1] * prev.y2;

			prev.x2 = prev.x1;
			prev.x1 = in[idx];

			prev.y2 = prev.y1;
			prev.y1 = y;

			out[idx] = y;
		}
	}
}


void BwFilter::_set(const u32 channels, const std::array<float, 2>& a, const std::array<float, 3>& b) {
	_channels = channels;
	_a = a;
	_b = b;
	_prev = decltype(_prev)(channels);
}

#define SET_PARAM(name) case FilterParams::name: if(value.type() == typeid(_##name)) { _##name = std::any_cast<decltype(_##name)>(std::move(value)); } break

pipeline::Layer& BwFilter::setParam(const u32 param, std::any value) {
	switch (param) {
		SET_PARAM(channels);
		SET_PARAM(cutoff);
		SET_PARAM(sampleRate);
		SET_PARAM(quality);
		SET_PARAM(gainDB);
		SET_PARAM(order);
	}
	refresh();
	return *this;
}

#define GET_PARAM(name) case FilterParams::name: result = _##name; break

std::any BwFilter::getParam(const u32 param) {
	std::any result;

	switch (param) {
		GET_PARAM(channels);
		GET_PARAM(cutoff);
		GET_PARAM(sampleRate);
		GET_PARAM(quality);
		GET_PARAM(gainDB);
		GET_PARAM(order);
	}

	return result;
}

}