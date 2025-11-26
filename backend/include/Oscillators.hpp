#include <oscillators/Oscillator.hpp>
#include <oscillators/PhaseOscillator.hpp>
#include <oscillators/SineOscillator.hpp>
#include <oscillators/SawtoothOscillator.hpp>
#include <oscillators/SquareOscillator.hpp>
#include <oscillators/TriangleOscillator.hpp>
#include <oscillators/ModulatedOscillator.hpp>
#include <string>
#include <unordered_set>

namespace oscillators {
inline const std::unordered_set<std::string> reservedOscillators = {
    "sine", "square", "triangle", "sawtooth", "empty"
};

}
