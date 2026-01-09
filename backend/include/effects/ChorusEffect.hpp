#pragma once
#include <effects/Effect.hpp>

namespace effects {

    /// @brief Params used by the chorus effect
    struct ChorusParams {
        enum Value: u32 {
            baseDelayFactor, // tells how much the sound is delayed without lfo
            modFrequency, // lfo frequency
            modDepth, // tells how much the lfo affects the sound's delay
            bufferFrames, // number of frames that can be stored in the delay buffer
            feedback, // tells how much of the signal is stored in the buffer
            mix, // tells how much of the buffer is used in the audio output
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

    /// @brief Creates an illusion that the signal is played from multiple sources,
    /// each with slightly different phase shift
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
        f32 _mix = 0.5f;

    public:
        /// @brief Set one of params described in ChorusParams struct
        /// @param param param index
        /// @param value param value
        pipeline::Layer& setParam(const u32 param, std::any value) override;

        /// @brief Get one of params described in ChorusParams struct
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
        ChorusEffect(const u32 channels, const f32 sampleRate);

        /// @brief Gets the type of the effect
        EffectType::Value getEffectType() override;

        /// @brief Updates internal state depending on params
        void refresh() override;
    };

}