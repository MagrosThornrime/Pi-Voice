#include <gtest/gtest.h>
#include <fileio/SampleManager.hpp>
#include <unordered_set>
#include <cmath>

TEST(SampleManager, ListFiles) {
    fileio::SampleManager sampleManager("../resources/samples", 44100);

    auto samples = sampleManager.getSampleNames();
    auto samplesSet = std::unordered_set(samples.begin(), samples.end());

    auto wantedSamples = std::unordered_set<std::string>{
        "industry",  // industry.wav
        "summer",    // summer.wav
		"noise"		// noise.wav
    };
    ASSERT_EQ(samplesSet, wantedSamples);
}

TEST(SampleManager, AlternativeFiles) {
    fileio::SampleManager sampleManager("../resources/samples", 44100);

	sampleManager.changeSamplesDirectory("resources/samples_testing");

    auto samples = sampleManager.getSampleNames();
    auto samplesSet = std::unordered_set(samples.begin(), samples.end());

    auto wantedSamples = std::unordered_set<std::string>{
        "noise",  // noise.wav
    };
    ASSERT_EQ(samplesSet, wantedSamples);
}

TEST(SampleManager, SampleHalfNonZero) {
    fileio::SampleManager sampleManager("../resources/samples", 44100);
	auto output = sampleManager.getSample("industry");

	ASSERT_FALSE(output.empty());
    i32 nonZeroCount = 0;
    i32 half = (output.size() + 1) / 2;
    f32 eps = 1e-6f;
    for (f32 x : output) {
        if (std::fabs(x) > eps) {
			nonZeroCount++;
        }
    }
	ASSERT_GT(nonZeroCount, half);
}
