#include <oscillators/Oscillator.hpp>
#include <oscillators/PhaseOscillator.hpp>
#include <oscillators/SineOscillator.hpp>
#include <oscillators/SawtoothOscillator.hpp>
#include <oscillators/SquareOscillator.hpp>
#include <oscillators/TriangleOscillator.hpp>
#include <string>

namespace oscillators {
// @brief Oscillator types
enum OscillatorType {
    sine, sawtooth, square, triangle, empty
};

inline OscillatorType oscillatorFromString(const std::string& type){
    if (type == "empty"){
        return empty;
    }
    if (type == "sine"){
        return sine;
    }
    if (type == "triangle"){
        return triangle;
    }
    if (type == "sawtooth"){
        return sawtooth;
    }
    if (type == "square"){
        return square;
    }
    return empty;
}

}
