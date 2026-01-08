#pragma once
#include <effects/Effect.hpp>

namespace effects {

    /// @brief Params used by the saturate effect
    struct SaturateParams {
        enum Value: u32 {
            drive, // tells how strong the signal is compressed
            _count,
        };

        template<Value P>
        struct _Type {
            using type = float;
        };

        template<Value P>
        using type = _Type<P>::type;
    };

    /// @brief Compresses the signal, reducing the difference in loudness between the loudest and quietest parts
    class SaturateEffect : public Effect {

        f32 _drive = 1.0f;

    public:
        /// @brief Set one of params described in SaturateParams struct
        /// @param param param index
        /// @param value param value
        pipeline::Layer& setParam(const u32 param, std::any value) override;

        /// @brief Get one of params described in SaturateParams struct
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
        SaturateEffect(const u32 channels, const f32 sampleRate);

        /// @brief Gets the type of the effect
        EffectType::Value getEffectType() override;

        /// @brief Updates internal state depending on params
        void refresh() override;
    };

}