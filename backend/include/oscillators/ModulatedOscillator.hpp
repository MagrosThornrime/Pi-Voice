#pragma once
#include <fileio/SampleManager.hpp>
#include <Types.hpp>
#include <vector>
#include <oscillators/Oscillator.hpp>


namespace oscillators {
/// @brief Takes a sound and modulates it based on given frequency
class ModulatedOscillator : public Oscillator {

	f32 _index = 0.0f;

	f32 _stepFactor;

	const std::vector<f32>& _sample;

public:

	void reset() override;

    ModulatedOscillator(f32 sampleRate, i32 voiceNumber, const std::vector<f32>& sample);

	/// @brief Updates the oscillator's state for the next frame
    void advance() override;

    /// @brief Calculates next sample's value
    f32 getNextSample() override;
};
}