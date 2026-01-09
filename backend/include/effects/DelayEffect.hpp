#pragma once
#include <effects/Effect.hpp>

namespace effects {

/// @brief Params used by the delay effect
struct DelayParams {
	enum Value: u32 {
		bufferFrames, // size of the delay buffer
    	feedback, // tells how much of the input is stored into the delay buffer
    	mix, // tells how much of the buffer is mixed into the output
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
    f32 _mix = 0.5f;

public:
	/// @brief Set one of params described in DelayParams struct
	/// @param param param index
	/// @param value param value
    pipeline::Layer& setParam(const u32 param, std::any value) override;

	/// @brief Get one of params described in DelayParams struct
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
    DelayEffect(const u32 channels, const f32 sampleRate);

	/// @brief Gets the type of the effect
    EffectType::Value getEffectType() override;

	/// @brief Updates internal state depending on params
	void refresh() override;
};

}