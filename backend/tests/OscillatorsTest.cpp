#include <gtest/gtest.h>
#include <oscillators/SineOscillator.hpp>
#include <oscillators/TriangleOscillator.hpp>
#include <oscillators/SquareOscillator.hpp>
#include <oscillators/NoiseOscillator.hpp>
#include <oscillators/SawtoothOscillator.hpp>

TEST(Oscillators, SineInRange) {
    SineOscillator oscillator(44100.0f);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(sample > -1.0f && sample < 1.0f);
    }
}

TEST(Oscillators, TriangleInRange) {
    TriangleOscillator oscillator(44100.0f);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(sample > -1.0f && sample < 1.0f);
    }
}

TEST(Oscillators, SquareInRange) {
    SquareOscillator oscillator(44100.0f);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(sample > -1.0f && sample < 1.0f);
    }
}

TEST(Oscillators, SawtoothInRange) {
    SawtoothOscillator oscillator(44100.0f);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(sample > -1.0f && sample < 1.0f);
    }
}

TEST(Oscillators, NoiseInRange) {
    NoiseOscillator oscillator(44100.0f);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(sample > -1.0f && sample < 1.0f);
    }
}