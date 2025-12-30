#pragma once
#include <effects/Effect.hpp>

namespace effects {

class DelayEffect : public Effect {

    std::vector<f32> _buffer;
    u32 _index = 0;

	u32 _delayTime = 10000;
    f32 _feedback = 0.1f;
    f32 _wetAmount = 0.5f;

    void _set(const u32 channels, const u32 delayTime, const f32 feedback, const f32 wetAmount);

public:
    pipeline::Layer& setParam(const u32 param, std::any value) override;
    std::any getParam(const u32 param) override;

    void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) override;

    DelayEffect(const u32 channels, const u32 delayTime, const f32 feedback, const f32 wetAmount);

	void refresh() override;
};

}