#pragma once
#include <effects/Effect.hpp>

namespace effects {

    class ModulatorEffect : public Effect {

        f32 _modFrequency = 30.0f;

        f32 _phaseIncrement;
        f32 _phase = 0.0f;

        void _set(const u32 channels, const f32 modFrequency);

    public:
        pipeline::Layer& setParam(const u32 param, std::any value) override;
        std::any getParam(const u32 param) override;

        void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) override;

        ModulatorEffect(const u32 channels, const f32 modFrequency);

        void refresh() override;
    };

}