#pragma once
#include <effects/Effect.hpp>

namespace effects {

    struct SaturateParams {
        enum Value: u32 {
            drive,
            _count,
        };

        template<Value P>
        struct _Type {
            using type = float;
        };

        template<Value P>
        using type = _Type<P>::type;
    };

    class SaturateEffect : public Effect {

        f32 _drive = 1.0f;

    public:
        pipeline::Layer& setParam(const u32 param, std::any value) override;
        std::any getParam(const u32 param) override;

        void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) override;

        SaturateEffect(const u32 channels, const f32 sampleRate);

        EffectType::Value getEffectType() override;

        void refresh() override;
    };

}