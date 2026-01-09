#pragma once

#include <Types.hpp>

namespace effects {

/// @brief Defines all effects usable by the synthesiser
struct EffectType {
    enum Value: u32 {
        chorus,
        delay,
        reverb,
        robotify,
        phaser,
        saturate,
        bitcrush,
    };
};

}