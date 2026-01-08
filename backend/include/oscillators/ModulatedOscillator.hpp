#pragma once
#include <fileio/SampleManager.hpp>
#include <Types.hpp>
#include <vector>
#include <oscillators/Oscillator.hpp>


namespace oscillators {
/// @brief Takes a sound and modulates it based on given frequency
class ModulatedOscillator : public Oscillator {

    /// @brief Index in the sample
	f32 _index = 0.0f;

    /// @brief Tells how fast the index changes
	f32 _step;

    /// @brief Preloaded sample that will be modulated
	const std::vector<f32>& _sample;

public:

    /// @brief Resets oscillator
	void reset() override;

    /// @brief Constructor
    ModulatedOscillator(f32 sampleRate, i32 voiceNumber, const std::vector<f32>& sample);

	/// @brief Updates the oscillator's state for the next frame
    void advance() override;

    /// @brief Calculates next sample's value
    f32 getNextSample() override;

    /// @brief Sets music note for the oscillator
    void setNote(i32 voiceNumber) override;

	/// @brief Tells if it's an empty oscillator
	bool isEmpty() override;
};
}