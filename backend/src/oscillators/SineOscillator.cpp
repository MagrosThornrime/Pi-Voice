#include <oscillators/SineOscillator.hpp>
#include <cmath>

float SineOscillator::getNextSample() {
    return std::sin(_phase);
}