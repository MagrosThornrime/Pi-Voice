#include <filters/BiquadFilter.hpp>
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

BiquadFilter::BiquadFilter(const u32 channels, const f32 sampleRate) : Layer(channels, sampleRate) {
	_prev = decltype(_prev)(_channels);
}

std::shared_ptr<BiquadFilter> BiquadFilter::create(FilterType::Value filter, const u32 channels, const f32 sampleRate) {
	std::shared_ptr<BiquadFilter> result = nullptr;
	switch (filter) {
		case FilterType::allPass: result = std::make_shared<AllPassFilter>(channels, sampleRate); break;
		case FilterType::bandPass: result = std::make_shared<BandPassFilter>(channels, sampleRate); break;
		case FilterType::highPass: result = std::make_shared<HighPassFilter>(channels, sampleRate); break;
		case FilterType::highShelf: result = std::make_shared<HighShelfFilter>(channels, sampleRate); break;
		case FilterType::lowPass: result = std::make_shared<LowPassFilter>(channels, sampleRate); break;
		case FilterType::lowShelf: result = std::make_shared<LowShelfFilter>(channels, sampleRate); break;
		case FilterType::notch: result = std::make_shared<NotchFilter>(channels, sampleRate); break;
		case FilterType::peakingEQ: result = std::make_shared<PeakingEQFilter>(channels, sampleRate); break;
	}
	if (result) {
		result->refresh();
	}

	return result;
}

void BiquadFilter::processSound(std::vector<f32>& inputBuffer,
	std::vector<f32>& outputBuffer,
	u32 frames) {

	u32 idx = 0;
	for (u32 i = 0; i < frames; ++i) {
		for (u32 ch = 0; ch < _channels; ++ch, ++idx) {

			auto& prev = _prev[ch];

			float y =
				_b[0] * inputBuffer[idx] +
				_b[1] * prev.x1 +
				_b[2] * prev.x2 -
				_a[0] * prev.y1 -
				_a[1] * prev.y2;

			prev.x2 = prev.x1;
			prev.x1 = inputBuffer[idx];

			prev.y2 = prev.y1;
			prev.y1 = y;

			outputBuffer[idx] = y;
		}
	}
}

#define SET_PARAM(name) case FilterParams::name: if(value.type() == typeid(_##name)) { _##name = std::any_cast<decltype(_##name)>(std::move(value)); } break

pipeline::Layer& BiquadFilter::setParam(const u32 param, std::any value) {
	switch (param) {
		SET_PARAM(cutoff);
		SET_PARAM(quality);
		SET_PARAM(gainDB);
		SET_PARAM(order);
	}
	refresh();
	return *this;
}

#define GET_PARAM(name) case FilterParams::name: result = _##name; break

std::any BiquadFilter::getParam(const u32 param) {
	std::any result;

	switch (param) {
		GET_PARAM(cutoff);
		GET_PARAM(quality);
		GET_PARAM(gainDB);
		GET_PARAM(order);
	}

	return result;
}

}