#pragma once

#include "Types.hpp"
#include <pipeline/Layer.hpp>

namespace effects {
    class Effect: public pipeline::Layer {
    public:
        static std::shared_ptr<Effect> create(EffectType::Value effect);

        Effect() = default;
        explicit Effect(const u32 channels);

    protected:
        u32 _channels = 2;
        f32 _sampleRate = 44100;

        void _set(const u32 channels);
    };
}