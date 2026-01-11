#include <gtest/gtest.h>
#include <pipeline/Pipeline.hpp>
#include <polyphonic/VoiceManager.hpp>
#include <fileio/FileRecorder.hpp>
#include <sequencer/Sequencer.hpp>
#include <fileio/SampleManager.hpp>
#include <filters/HighPassFilter.hpp>
#include <filesystem>

using namespace pipeline;
namespace fs = std::filesystem;

class PipelineTest : public ::testing::Test {
protected:
    void SetUp() override {
        fs::create_directory("test_samples");
        fs::create_directory("test_output");

        sampleManager = std::make_shared<fileio::SampleManager>("test_samples", 44100);

        voiceManager = std::make_shared<polyphonic::VoiceManager>(
            1, 44100.0f, 2, sampleManager
        );

        recorder = std::make_shared<fileio::FileRecorder>(
            44100, 2, 1.0f, true
        );
        recorder->setOutputDirectory("test_output");

        sequencer = std::make_shared<seq::Sequencer>(sampleManager);

        pipeline = std::make_unique<Pipeline>(
            128,
            2,
            44100.0f,
            voiceManager,
            recorder,
            sequencer
        );
    }

    void TearDown() override {
        recorder->stop();
        fs::remove_all("test_samples");
        fs::remove_all("test_output");
    }

    std::shared_ptr<fileio::SampleManager> sampleManager;
    std::shared_ptr<polyphonic::VoiceManager> voiceManager;
    std::shared_ptr<fileio::FileRecorder> recorder;
    std::shared_ptr<seq::Sequencer> sequencer;
    std::unique_ptr<Pipeline> pipeline;
};

TEST_F(PipelineTest, StartsWithNoLayers) {
    ASSERT_EQ(pipeline->length(), 0u);
}

TEST_F(PipelineTest, AddLayerIncreasesLength) {
    auto filter = std::make_shared<filters::HighPassFilter>(2, 44100.0f);
    pipeline->add(filter, std::nullopt);
    ASSERT_EQ(pipeline->length(), 1u);
}

TEST_F(PipelineTest, RemoveLayerDecreasesLength) {
    auto filter = std::make_shared<filters::HighPassFilter>(2, 44100.0f);
    pipeline->add(filter, std::nullopt);
    pipeline->remove(0);
    ASSERT_EQ(pipeline->length(), 0u);
}

TEST_F(PipelineTest, MoveLayerReorders) {
    auto f1 = std::make_shared<filters::HighPassFilter>(2, 44100.0f);
    auto f2 = std::make_shared<filters::HighPassFilter>(2, 44100.0f);

    pipeline->add(f1, std::nullopt);
    pipeline->add(f2, std::nullopt);

    pipeline->move(0, 1);

    ASSERT_EQ(pipeline->get(1), f1);
}

TEST_F(PipelineTest, SwapLayers) {
    auto f1 = std::make_shared<filters::HighPassFilter>(2, 44100.0f);
    auto f2 = std::make_shared<filters::HighPassFilter>(2, 44100.0f);

    pipeline->add(f1, std::nullopt);
    pipeline->add(f2, std::nullopt);

    pipeline->swap(0, 1);

    ASSERT_EQ(pipeline->get(0), f2);
    ASSERT_EQ(pipeline->get(1), f1);
}

TEST_F(PipelineTest, InvalidRemoveReturnsNull) {
    ASSERT_EQ(pipeline->remove(0), nullptr);
}

TEST_F(PipelineTest, InvalidGetReturnsNull) {
    ASSERT_EQ(pipeline->get(10), nullptr);
}

TEST_F(PipelineTest, SetAmplitudeOutOfRangeThrows) {
    EXPECT_THROW(pipeline->setAmplitude(-0.1f), std::invalid_argument);
    EXPECT_THROW(pipeline->setAmplitude(1.1f), std::invalid_argument);
}

TEST_F(PipelineTest, GettersReturnCorrectValues) {
    ASSERT_EQ(pipeline->getChannels(), 2u);
    ASSERT_FLOAT_EQ(pipeline->getSampleRate(), 44100.0f);
}
