#include <gtest/gtest.h>
#include <fileio/SampleManager.hpp>

TEST(SampleManager, ReadFiles) {
    fileio::SampleManager sampleManager("samples");

    auto samples = sampleManager.getSampleNames();
    auto samplesSet = std::unordered_set(samples.begin(), samples.end());

    auto wantedSamples = std::unordered_set<std::string>{
        "industry", "summer"
    };
    ASSERT_EQ(samplesSet, wantedSamples);

}