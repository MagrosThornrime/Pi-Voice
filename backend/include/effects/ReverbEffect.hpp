#pragma once
#include <effects/Effect.hpp>
#include <array>

namespace effects {

    struct ReverbParams {
        enum Value: u32 {
			bufferFrames,
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

#define PARAM_TYPE(param, T) template<> struct ReverbParams::_Type<param> { using type = T; }

    PARAM_TYPE(ReverbParams::bufferFrames, u32);

#undef PARAM_TYPE

    class ReverbEffect : public Effect {

        const std::array<f32, 8> _bufferFramesFactors = {
            1.4983f,
            1.5413f,
            1.4281f,
            1.3829f,
            1.2553f,
            1.3127f,
            1.1489f,
            1.0867f
        };

        std::array<std::vector<f32>, 8> _buffers{};
        std::array<u32, 8> _indices{};
		u32 _bufferFrames = 10000;

        f32 _feedback = 0.1f;
        f32 _wetAmount = 0.5f;

        void _set(const u32 bufferFrames, const f32 feedback, const f32 wetAmount);

    public:
        pipeline::Layer& setParam(const u32 param, std::any value) override;
        std::any getParam(const u32 param) override;

        void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) override;

        ReverbEffect(const u32 channels, const f32 sampleRate);

        EffectType::Value getEffectType() override;

        void refresh() override;
    };

}