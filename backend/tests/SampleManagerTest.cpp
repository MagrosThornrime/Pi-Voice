#include <gtest/gtest.h>
#include <fileio/SampleManager.hpp>
#include <unordered_set>
#include <cmath>

TEST(SampleManager, ListFiles) {
    fileio::SampleManager sampleManager("res/samples_testing", 44100);

    auto samples = sampleManager.getSampleNames();
    auto samplesSet = std::unordered_set(samples.begin(), samples.end());

    auto wantedSamples = std::unordered_set<std::string>{
    	"industry", "summer", "meow", "sine", "square", "empty", "triangle", "sawtooth"
    };

    ASSERT_EQ(samplesSet, wantedSamples);
}

TEST(SampleManager, AlternativeFiles) {
    fileio::SampleManager sampleManager("res/samples_testing", 44100);

	sampleManager.changeSamplesDirectory("res/samples");

    auto samples = sampleManager.getSampleNames();
    auto samplesSet = std::unordered_set(samples.begin(), samples.end());

	auto wantedSamples = std::unordered_set<std::string>{
		"bonus 1", "meow", "achievement", "sword", "sine",
		"square", "empty", "bonus 2",
	    "triangle", "explosion", "character", "sawtooth"
    };

    ASSERT_EQ(samplesSet, wantedSamples);
}

TEST(SampleManager, SampleHalfNonZero) {
    fileio::SampleManager sampleManager("res/samples_testing", 44100);
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
