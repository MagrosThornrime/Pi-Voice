#pragma once
#include <effects/Effect.hpp>

namespace effects {

    struct BitcrushParams {
        enum Value: u32 {
            bits,
            _count,
        };

        template<Value P>
        struct _Type {
            using type = float;
        };

        template<Value P>
        using type = _Type<P>::type;
    };

#define PARAM_TYPE(param, T) template<> struct BitcrushParams::_Type<param> { using type = T; }

    PARAM_TYPE(BitcrushParams::bits, u32);

#undef PARAM_TYPE

    class BitcrushEffect : public Effect {

        u32 _bits = 1;
        u32 _levels;

    public:
        pipeline::Layer& setParam(const u32 param, std::any value) override;
        std::any getParam(const u32 param) override;

        void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) override;

        BitcrushEffect(const u32 channels, const f32 sampleRate);

        EffectType::Value getEffectType() override;

        void refresh() override;
    };

}