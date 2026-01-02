#pragma once

#include "Types.hpp"
#include <pipeline/Layer.hpp>
#include <effects/EffectTypes.hpp>
#include <memory>

namespace effects {
    class Effect: public pipeline::Layer {
    public:

        static std::shared_ptr<Effect> create(EffectType::Value effect, const u32 channels, const f32 sampleRate);

        Effect(const u32 channels, const f32 sampleRate);

        virtual EffectType::Value getEffectType() = 0;

    };
}