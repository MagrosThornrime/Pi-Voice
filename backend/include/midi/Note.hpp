#pragma once

#include <Types.hpp>
#include <string_view>

namespace midi {

namespace _details {

// initializer for note
struct NotesInitializer {
    NotesInitializer() noexcept;
};
extern NotesInitializer notes;

}

/// @brief Note data
struct Note {

	/// @brief Base frequency
	static constexpr float baseFreq = 440.f;

	/// @brief Note midi key number
    i32 num;
	/// @brief Note name
    char name[8];
	/// @brief Note frequency
    float freq;

	/// @brief Returns note frequency with base frequency different than 440Hz
	/// @param baseFreq base frequency
	float freqWithBase(float baseFreq) const noexcept;
};
/// @brief Array of notes, where i-th note is i-th midi key
extern Note note[128];

}