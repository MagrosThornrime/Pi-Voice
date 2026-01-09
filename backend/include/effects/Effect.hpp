#pragma once

#include "Types.hpp"
#include <pipeline/Layer.hpp>
#include <effects/EffectTypes.hpp>
#include <memory>
#include <algorithm>

namespace effects {
    /// @brief Layer type used for audio manipulation not focused on filtering
    class Effect: public pipeline::Layer {
    public:

        /// @brief Returns a new instance of chosen effect type
        /// @param effect Type of the effect
        /// @param channels number of channels
        /// @param sampleRate sound's sample rate
        static std::shared_ptr<Effect> create(EffectType::Value effect, const u32 channels, const f32 sampleRate);

        /// @brief Constructor
        /// @param channels number of channels
        /// @param sampleRate sound's sample rate
        Effect(const u32 channels, const f32 sampleRate);

        /// @brief Returns type of the current effect
        virtual EffectType::Value getEffectType() = 0;

    };
}