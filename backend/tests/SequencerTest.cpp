#include <gtest/gtest.h>
#include <sequencer/Sequencer.hpp>
#include <fileio/SampleManager.hpp>
#include <filesystem>

namespace fs = std::filesystem;

class SequencerTest : public ::testing::Test {
protected:
    void SetUp() override {
        fs::create_directory("test_samples");
        sampleManager = std::make_shared<fileio::SampleManager>("test_samples", 44100);
        sequencer = std::make_unique<seq::Sequencer>(sampleManager);
    }

    void TearDown() override {
        fs::remove_all("test_samples");
    }

    std::shared_ptr<fileio::SampleManager> sampleManager;
    std::unique_ptr<seq::Sequencer> sequencer;
};

TEST_F(SequencerTest, StartsInactive) {
    ASSERT_FALSE(sequencer->isActive());
}

TEST_F(SequencerTest, ActivateEnablesSequencer) {
    sequencer->activate();
    ASSERT_TRUE(sequencer->isActive());
}

TEST_F(SequencerTest, AddSampleIncreasesLength) {
    std::vector<f32> sample(44100, 0.5f);
    sequencer->addSample(sample);
    ASSERT_EQ(sequencer->length(), 1u);
}

TEST_F(SequencerTest, RemoveSampleDecreasesLength) {
    std::vector<f32> sample(100);
    sequencer->addSample(sample);
    sequencer->removeSample(0);
    ASSERT_EQ(sequencer->length(), 0u);
}

TEST_F(SequencerTest, ClearRemovesAllSamples) {
    sequencer->addSample(std::vector<f32>(10));
    sequencer->addSample(std::vector<f32>(10));
    sequencer->clear();
    ASSERT_EQ(sequencer->length(), 0u);
}

TEST_F(SequencerTest, SampleLengthIsCorrect) {
    std::vector<f32> sample(44100 * 2); // 1 second stereo
    sequencer->addSample(sample);

    float len = sequencer->sampleLength(0);
    ASSERT_NEAR(len, 1.0f, 0.001f);
}
