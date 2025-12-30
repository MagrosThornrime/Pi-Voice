#pragma once
#include <effects/Effect.hpp>

namespace effects {

    class RobotifyEffect : public Effect {

        f32 _modFrequency = 30.0f;

        f32 _phaseIncrement;
        f32 _phase;

        void _set(const u32 channels, const f32 modFrequency);

    public:
        pipeline::Layer& setParam(const u32 param, std::any value) override;
        std::any getParam(const u32 param) override;

        void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) override;

        RobotifyEffect(const u32 channels, const f32 modFrequency);

        void refresh() override;
    };

}