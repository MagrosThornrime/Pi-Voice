#pragma once

#include "Types.hpp"
#include <portaudiocpp/PortAudioCpp.hxx>
#include <array>
#include <vector>

// Bw - Butterworth
class BwFilter: public portaudio::CallbackInterface {
public:
	BwFilter() = default;
	BwFilter(const u32 channels, const std::array<float, 2>& a, const std::array<float, 3>& b);

	int paCallbackFun(const void* inputBuffer, void* outputBuffer, unsigned long numFrames, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags) override;

protected:
	struct Prev {
		float x1{}, x2{}, y1{}, y2{};
	};

	std::vector<Prev> _prev;
	std::array<float, 2> _a;
	std::array<float, 3> _b;
	u32 _channels;

	void _set(const u32 channels, const std::array<float, 2>& a, const std::array<float, 3>& b);
};