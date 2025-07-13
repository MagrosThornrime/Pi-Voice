#include <oscillators/SineOscillator.hpp>
#include <cmath>

namespace oscillators {
f32 SineOscillator::getNextSample() {
    return std::sin(_phase);
}
}
