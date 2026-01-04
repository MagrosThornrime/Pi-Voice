#pragma once

#include <Types.hpp>

namespace effects {

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