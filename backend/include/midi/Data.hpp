#pragma once

#include <Types.hpp>
#include "Note.hpp"
#include <Assert.hpp>

namespace midi {

struct Data {
    enum Status : u8 {
        noteOff = 0x80,
        noteOn = 0x90,
        
        // below unsupported
        polyphonicAftertouch = 0xA0,
        controlChange = 0xB0,
        programChange = 0xC0,
        channelAftertouch = 0xD0,
        pitchWheel = 0xE0,
    };

    double time{};
    u8 bytes[3]{};

    inline bool valid() const noexcept {
        return bytes[0] || bytes[1] || bytes[2];
    }
    inline operator bool() const noexcept {
        return valid();
    }

    constexpr Status type() const noexcept {
        return Status(bytes[0] & (u8)0xF0);
    }
    constexpr u8 channel() const noexcept {
        return bytes[0] & (u8)0xF;
    }
    
	inline bool hasNote() const noexcept {
		return type() == noteOff or type() == noteOn or type() == polyphonicAftertouch;
	}
    inline Note note() const {
        ASSERT(hasNote());
        return midi::note[bytes[1]];
    }
    inline f32 velocity() const {
        ASSERT(hasNote());
        return bytes[2] / 127.f;
    }
};

}