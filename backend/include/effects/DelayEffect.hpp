#pragma once
#include <effects/Effect.hpp>

namespace effects {

struct DelayParams {
	enum Value: u32 {
		bufferFrames,
    	feedback,
    	wetAmount,
		_count,
	};

	template<Value P>
	struct _Type {
		using type = float;
	};

	template<Value P>
	using type = _Type<P>::type;
};

#define PARAM_TYPE(param, T) template<> struct DelayParams::_Type<param> { using type = T; }

PARAM_TYPE(DelayParams::bufferFrames, u32);

#undef PARAM_TYPE

class DelayEffect : public Effect {

    std::vector<f32> _buffer;
    u32 _index = 0;

	u32 _bufferFrames = 10000;
    f32 _feedback = 0.1f;
    f32 _wetAmount = 0.5f;

    void _set(const u32 bufferFrames, const f32 feedback, const f32 wetAmount);

public:
    pipeline::Layer& setParam(const u32 param, std::any value) override;
    std::any getParam(const u32 param) override;

    void processSound(std::vector<f32>& inputBuffer, std::vector<f32>& outputBuffer, u32 frames) override;

    DelayEffect(const u32 channels, const f32 sampleRate);

    EffectType::Value getEffectType() override;

	void refresh() override;
};

}