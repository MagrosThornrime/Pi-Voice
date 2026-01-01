#pragma once
#include <effects/Effect.hpp>
#include <array>

namespace effects {

    struct ReverbParams {
        enum Value: u32 {
            feedback,
            wetAmount,
            _count,
        };

        template<Value P>
        struct _Type {
            using type = float;
        };

        template<Value P>
        using type = _Type<P>::type;
    };

    class ReverbEffect : public Effect {

        const std::array<u32, 8> _bufferSizes = {
            14983,
            15413,
            14281,
            13829,
            12553,
            13127,
            11489,
            10867
        };

        std::array<std::vector<f32>, 8> _buffers{};
        std::array<u32, 8> _indices{};

        f32 _feedback = 0.1f;
        f32 _wetAmount = 0.5f;

        void _set(const u32 channels, const f32 feedback, const f32 wetAmount);

    public:
        pipeline::Layer& setParam(const u32 param, std::any value) override;
        std::any getParam(const u32 param) override;

        void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) override;

        ReverbEffect(const u32 channels, const f32 feedback, const f32 wetAmount);
        ReverbEffect();

        EffectType::Value getEffectType() override;

        void refresh() override;
    };

}