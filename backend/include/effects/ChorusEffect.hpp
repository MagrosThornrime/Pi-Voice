#pragma once
#include <effects/Effect.hpp>

namespace effects {

    class ChorusEffect : public Effect {

        std::vector<f32> _buffer;
        u32 _index = 0;
        f32 _baseDelayFactor = 0.03f;

        f32 _phase = 0.0f;

        f32 _modFrequency = 1.5f;
        f32 _modDepth = 0.005f;

        u32 _bufferFrames = 10000;
        f32 _feedback = 0.1f;
        f32 _wetAmount = 0.5f;

        void _set(const u32 channels, const u32 bufferFrames, const f32 feedback, const f32 wetAmount,
            const f32 baseDelayFactor, const f32 modFrequency, const f32 modDepth);

    public:
        pipeline::Layer& setParam(const u32 param, std::any value) override;
        std::any getParam(const u32 param) override;

        void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) override;

        ChorusEffect(const u32 channels, const u32 bufferFrames, const f32 feedback, const f32 wetAmount,
            const f32 baseDelayFactor, const f32 modFrequency, const f32 modDepth);

        void refresh() override;
    };

}