#include <gtest/gtest.h>
#include <oscillators/SineOscillator.hpp>
#include <oscillators/TriangleOscillator.hpp>
#include <oscillators/SquareOscillator.hpp>
#include <oscillators/NoiseOscillator.hpp>
#include <oscillators/SawtoothOscillator.hpp>

TEST(Oscillators, SineInRange) {
    oscillators::SineOscillator oscillator(44100.0f);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(sample > -1.0f && sample < 1.0f) << "Sample out of range: " << sample;
    }
}

TEST(Oscillators, TriangleInRange) {
    oscillators::TriangleOscillator oscillator(44100.0f);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(sample > -1.0f && sample < 1.0f) << "Sample out of range: " << sample;
    }
}

TEST(Oscillators, SquareInRange) {
    oscillators::SquareOscillator oscillator(44100.0f);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(sample > -1.0f && sample < 1.0f) << "Sample out of range: " << sample;
    }
}

TEST(Oscillators, SawtoothInRange) {
    oscillators::SawtoothOscillator oscillator(44100.0f);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(sample > -1.0f && sample < 1.0f) << "Sample out of range: " << sample;
    }
}

TEST(Oscillators, NoiseInRange) {
    oscillators::NoiseOscillator oscillator(44100.0f);
    for(int i = 0; i < 1000; i++) {
        oscillator.advance();
        float sample = oscillator.getNextSample();
        ASSERT_TRUE(sample > -1.0f && sample < 1.0f) << "Sample out of range: " << sample;
    }
}
