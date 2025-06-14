#pragma once

#include <Types.hpp>
#include "Note.hpp"
#include <Assert.hpp>

namespace midi {

/// @brief Data type returned by midi readers
struct Data {

    /// @brief Status types
    enum Status : u8 {
        /// @brief Key was released
        noteOff = 0x80,
        /// @brief Key was pressed
        noteOn = 0x90,
        
        // below unsupported
        polyphonicAftertouch = 0xA0,
        controlChange = 0xB0,
        programChange = 0xC0,
        channelAftertouch = 0xD0,
        pitchWheel = 0xE0,
    };

    /// @brief Time since last event
    double time{};
	/// @brief Data of event
	/// @details First element is status + channel, rest are data
    u8 bytes[3]{};

    /// @brief Checks if Data instance is valid
    inline bool valid() const noexcept {
		return bytes[0] || bytes[1] || bytes[2];
    }
	/// @brief Checks if Data instance is valid
    inline operator bool() const noexcept {
        return valid();
    }

    /// @brief Returns status from data (4 MSB)
    constexpr Status status() const noexcept {
        return Status(bytes[0] & (u8)0xF0);
    }
    /// @brief Returns channel from data (4 LSB)
    constexpr u8 channel() const noexcept {
        return bytes[0] & (u8)0xF;
    }
    
	/// @brief Checks if holds note data
	inline bool hasNote() const noexcept {
		return status() == noteOff or status() == noteOn or status() == polyphonicAftertouch;
	}
	/// @brief Checks if holds note data
	inline bool hasVelocity() const noexcept {
		return status() == noteOff or status() == noteOn;
	}
	/// @brief Returns note
    inline Note note() const {
        ASSERT(hasNote());
        return midi::note[bytes[1]];
    }
    /// @brief Returns normalized velocity
    inline f32 velocity() const {
        ASSERT(hasVelocity());
        return bytes[2] / 127.f;
    }
};

}