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
class BiquadFilter: public pipeline::Layer {
public:
	static std::shared_ptr<BiquadFilter> create(FilterType::Value filter, const u32 channels, const f32 sampleRate);

	BiquadFilter(const u32 channels, const f32 sampleRate);

	pipeline::Layer& setParam(const u32 param, std::any value) override;
	std::any getParam(const u32 param) override;

	void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) override;

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
};
}