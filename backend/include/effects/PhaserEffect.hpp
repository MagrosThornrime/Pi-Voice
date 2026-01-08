#pragma once
#include <effects/Effect.hpp>
#include <effects/AllPassStage.hpp>
#include <array>

namespace effects {

    /// @brief Params used by the phaser effect
    struct PhaserParams {
        enum Value: u32 {
            minHz, // mininum phaser sweep frequency
            rangeHz, // range from the minimum to maximum phaser sweep frequency
            rateHz, // phaser sweep rate
            feedback, // tells how much of the signal is put into the buffer
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

	/// @brief Implements the phaser sweep effect
    class PhaserEffect : public Effect {

        f32 _minHz = 440.0f;
        f32 _rangeHz = 1160.0f;
        f32 _rateHz = 0.5f;
        f32 _feedback = 0.1f;
        f32 _mix = 0.5f;

        f32 _minCoeff, _maxCoeff;
        f32 _lfoIncrement;
        f32 _lfoPhase = 0.0f;
        std::vector<f32> _feedbackBuffer;

        std::array<AllPassStage, 6> _allPassStages;

    public:
        /// @brief Set one of params described in PhaserEffect struct
        /// @param param param index
        /// @param value param value
        pipeline::Layer& setParam(const u32 param, std::any value) override;

        /// @brief Get one of params described in PhaserEffect struct
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
        PhaserEffect(const u32 channels, const f32 sampleRate);

        /// @brief Gets the type of the effect
        EffectType::Value getEffectType() override;

        /// @brief Updates internal state depending on params
        void refresh() override;
    };

}