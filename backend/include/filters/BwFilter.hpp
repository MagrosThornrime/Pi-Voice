#pragma once

#include "Types.hpp"
#include <pipeline/Layer.hpp>
#include <array>
#include <vector>
#include <numbers>
#include <memory>
#include "FilterTypes.hpp"

namespace filters {
// Bw - Butterworth
class BwFilter: public pipeline::Layer {
public:
	static std::shared_ptr<BwFilter> create(FilterType::Value filter);

	BwFilter() = default;
	BwFilter(const u32 channels, const std::array<float, 2>& a, const std::array<float, 3>& b);

	pipeline::Layer& setParam(const u32 param, std::any value) override;
	std::any getParam(const u32 param) override;

	int paCallbackFun(const void* inputBuffer, void* outputBuffer, unsigned long numFrames, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags) override;

protected:
	struct Prev {
		float x1{}, x2{}, y1{}, y2{};
	};

	std::vector<Prev> _prev;
	std::array<f32, 2> _a;
	std::array<f32, 3> _b;
	u32 _channels = 2;
	f32 _cutoff = 1000;
	f32 _sampleRate = 44100;
	f32 _quality = std::numbers::sqrt2_v<f32> / 2;
	f32 _gainDB = 0;
	u32 _order = 2;

	void _set(const u32 channels, const std::array<float, 2>& a, const std::array<float, 3>& b);
};
}