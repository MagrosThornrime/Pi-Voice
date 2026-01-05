#pragma once
#include <effects/Effect.hpp>
#include <effects/AllPassStage.hpp>
#include <array>

namespace effects {

    struct PhaserParams {
        enum Value: u32 {
            minHz,
            rangeHz,
            rateHz,
            feedback,
            mix,
            _count,
        };

        template<Value P>
        struct _Type {
            using type = float;
        };

        template<Value P>
        using type = _Type<P>::type;
    };

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
        pipeline::Layer& setParam(const u32 param, std::any value) override;
        std::any getParam(const u32 param) override;

        void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) override;

        PhaserEffect(const u32 channels, const f32 sampleRate);

        EffectType::Value getEffectType() override;

        void refresh() override;
    };

}