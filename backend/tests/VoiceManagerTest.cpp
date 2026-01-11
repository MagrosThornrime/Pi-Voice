#include <gtest/gtest.h>
#include <polyphonic/VoiceManager.hpp>
#include <fileio/SampleManager.hpp>
#include <filesystem>
#include <vector>
#include <cmath>

using namespace polyphonic;
namespace fs = std::filesystem;

const float epsilon = 0.000001f;

class VoiceManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        fs::create_directory("test_samples");
        sampleManager = std::make_shared<fileio::SampleManager>("test_samples", 44100);

        voiceManager = std::make_unique<VoiceManager>(
            8,
            44100.0f,
            2,
            sampleManager
        );
    }

    void TearDown() override {
        fs::remove_all("test_samples");
    }

    std::shared_ptr<fileio::SampleManager> sampleManager;
    std::unique_ptr<VoiceManager> voiceManager;
};

TEST_F(VoiceManagerTest, StartsWithNoActiveVoices) {
    ASSERT_FALSE(voiceManager->hasActiveVoices());
}

TEST_F(VoiceManagerTest, TurnOnActivatesVoice) {
    voiceManager->turnOn(69);
    ASSERT_TRUE(voiceManager->hasActiveVoices());
}

TEST_F(VoiceManagerTest, GenerateSoundSilentWhenNoVoices) {
    std::vector<f32> buffer(512 * 2);
    voiceManager->generateSound(buffer, 512);

    for (auto s : buffer) {
        ASSERT_NEAR(s, 0.0f, epsilon);
    }
}
