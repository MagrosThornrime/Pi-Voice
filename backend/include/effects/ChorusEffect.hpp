#pragma once
#include <effects/Effect.hpp>

namespace effects {

    struct ChorusParams {
        enum Value: u32 {
            baseDelayFactor,
            modFrequency,
            modDepth,
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

#define PARAM_TYPE(param, T) template<> struct ChorusParams::_Type<param> { using type = T; }

    PARAM_TYPE(ChorusParams::bufferFrames, u32);

#undef PARAM_TYPE

    class ChorusEffect : public Effect {

        std::vector<f32> _buffer;
        u32 _index = 0;
        f32 _baseDelayFactor = 0.03f;

        f32 _phase = 0.0f;

        f32 _modFrequency = 1.5f;
        f32 _modDepth = 0.005f;

		f32 _phaseIncrement;

        u32 _bufferFrames = 10000;
        f32 _feedback = 0.1f;
        f32 _wetAmount = 0.5f;

    public:
        pipeline::Layer& setParam(const u32 param, std::any value) override;
        std::any getParam(const u32 param) override;

        void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) override;

        ChorusEffect(const u32 channels, const f32 sampleRate);

        EffectType::Value getEffectType() override;

        void refresh() override;
    };

}