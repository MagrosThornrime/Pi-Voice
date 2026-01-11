#include <gtest/gtest.h>
#include <Oscillators.hpp>
#include <cmath>

const float epsilon = 0.000001f;

TEST(Oscillators, SineInRange) {
    oscillators::SineOscillator oscillator(44100.0f, 69);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(std::abs(sample) < 1.0f + epsilon) << "Sample out of range: " << sample;
    }
}

TEST(Oscillators, TriangleInRange) {
    oscillators::TriangleOscillator oscillator(44100.0f, 69);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(std::abs(sample) < 1.0f + epsilon) << "Sample out of range: " << sample;
    }
}

TEST(Oscillators, SquareInRange) {
    oscillators::SquareOscillator oscillator(44100.0f, 69);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(std::abs(sample) < 1.0f + epsilon) << "Sample out of range: " << sample;
    }
}

TEST(Oscillators, SawtoothInRange) {
    oscillators::SawtoothOscillator oscillator(44100.0f, 69);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(std::abs(sample) < 1.0f + epsilon) << "Sample out of range: " << sample;
    }
}