#pragma once
#include <effects/Effect.hpp>

namespace effects {

    /// @brief Params used by the bitcrush effect
    struct BitcrushParams {
        enum Value: u32 {
            bits,  // desired bit depth of the signal
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

    /// @brief Reduces bit depth of the signal, so it can sound like in old, 8-bit retro games.
    class BitcrushEffect : public Effect {

        u32 _bits = 1;
        u32 _levels;

    public:
        /// @brief Set one of params described in BitcrushParams struct
        /// @param param param index
        /// @param value param value
        pipeline::Layer& setParam(const u32 param, std::any value) override;

        /// @brief Get one of params described in BitcrushParams struct
        /// @param param index
        std::any getParam(const u32 param) override;

        /// @brief Apply the effect to the signal
        /// @param inputBuffer input signal
        /// @param outputBuffer output signal
        /// @param frames number of frames affected
        void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) override;

        /// @brief Constructor
        /// @param channels number of channels
        /// @param sampleRate sounds's sample rate
        BitcrushEffect(const u32 channels, const f32 sampleRate);

        /// @brief Gets the type of the effect
        EffectType::Value getEffectType() override;

        /// @brief Updates internal state depending on params
        void refresh() override;
    };

}