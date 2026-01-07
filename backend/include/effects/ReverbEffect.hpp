#pragma once
#include <effects/Effect.hpp>
#include <effects/AllPassStage.hpp>
#include <array>

namespace effects {

    /// @brief Params used by the reverb effect
    struct ReverbParams {
        enum Value: u32 {
			bufferFrames, // base number of frames that can be stored in each buffer (excluding later modification)
            feedback, // tells how much of the signal is put into the buffers
            mix, // tells how much of the signal in buffers is put into the output
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

    /// @brief Creates an illusion of echo - sound waves reflecting and returning to the source over time
    class ReverbEffect : public Effect {

        /// @brief Buffer lengths are multiplied by it
        static constexpr std::array<f32, 8> _bufferFramesFactors = {
            1.4983f,
            1.5413f,
            1.4281f,
            1.3829f,
            1.2553f,
            1.3127f,
            1.1489f,
            1.0867f
        };

        /// @brief delay buffers
        std::array<std::vector<f32>, 8> _buffers{};

        /// @brief indices used by delay buffers
        std::array<u32, 8> _indices{};

		u32 _bufferFrames = 10000;

        f32 _feedback = 0.1f;
        f32 _mix = 0.5f;

        /// @brief Changes the frequencies affected by the AllPass filters stages
        const std::array<f32, 4> _allPassCoeffs = { 0.03f, 0.07f, 0.11f, 0.17f };
        std::array<AllPassStage, 4> _allPassStages;

    public:
        /// @brief Set one of params described in ReverbEffect struct
        /// @param param param index
        /// @param value param value
        pipeline::Layer& setParam(const u32 param, std::any value) override;

        /// @brief Get one of params described in ReverbParams struct
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
        ReverbEffect(const u32 channels, const f32 sampleRate);

        /// @brief Gets the type of the effect
        EffectType::Value getEffectType() override;

        /// @brief Updates internal state depending on params
        void refresh() override;
    };

}