#pragma once
#include <effects/Effect.hpp>

namespace effects {

    /// @brief Params used by the robotify effect
    struct RobotifyParams {
        enum Value: u32 {
            modFrequency, // lfo frequency
            _count,
        };

        template<Value P>
        struct _Type {
            using type = float;
        };

        template<Value P>
        using type = _Type<P>::type;
    };

    /// @brief Applies a distortion, modulating the signal's amplitude using a lfo
    class RobotifyEffect : public Effect {

        f32 _modFrequency = 30.0f;

        f32 _phaseIncrement;
        f32 _phase;

    public:
        /// @brief Set one of params described in RobotifyParams struct
        /// @param param param index
        /// @param value param value
        pipeline::Layer& setParam(const u32 param, std::any value) override;

        /// @brief Get one of params described in RobotifyParams struct
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
        RobotifyEffect(const u32 channels, const f32 sampleRate);

        /// @brief Gets the type of the effect
        EffectType::Value getEffectType() override;

        /// @brief Updates internal state depending on params
        void refresh() override;
    };

}