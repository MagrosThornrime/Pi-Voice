#include <oscillators/SineOscillator.hpp>
#include <cmath>

f32 SineOscillator::getNextSample() {
    return std::sin(_phase);
}