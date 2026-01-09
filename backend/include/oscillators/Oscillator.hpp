#pragma once
#include <portaudio.h>
#include <Types.hpp>

namespace oscillators {
/// @brief An interface for all oscillators
class Oscillator {
protected:
    /// @brief Sample rate of generated sound
    f32 _sampleRate;

	/// @brief Index of the note
	i32 _voiceNumber;

public:

	/// @brief Resets oscillator
	virtual void reset();

    /// @brief Constructor
    explicit Oscillator(f32 sampleRate, i32 voiceNumber);

    /// @brief Updates the oscillator's state for the next frame
    virtual void advance();

    /// @brief Calculates next sample's value
    virtual f32 getNextSample();

    /// @brief Sets music note for the oscillator
	virtual void setNote(i32 voiceNumber);

	/// @brief Tells if it's an empty oscillator
	virtual bool isEmpty();

};
}
