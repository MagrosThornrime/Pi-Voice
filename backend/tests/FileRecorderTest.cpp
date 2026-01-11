#include <gtest/gtest.h>
#include <fileio/FileRecorder.hpp>
#include <filesystem>
#include <thread>
#include <fstream>


using namespace fileio;
namespace fs = std::filesystem;

TEST(FileRecorder, SetValidOutputDirectory) {
    fs::create_directory("test_output");
    FileRecorder recorder(44100, 2, 1.0f, false);
    EXPECT_NO_THROW(recorder.setOutputDirectory("test_output"));
    fs::remove_all("test_output");
}

TEST(FileRecorder, SetInvalidOutputDirectoryThrows) {
    FileRecorder recorder(44100, 2, 1.0f, false);
    std::ofstream ofs("not_a_dir");
    ofs.close();
    EXPECT_THROW(recorder.setOutputDirectory("not_a_dir"), std::invalid_argument);
    fs::remove("not_a_dir");
}

TEST(FileRecorder, StartAndStopDoesNotThrow) {
    fs::create_directory("test_output");
    FileRecorder recorder(44100, 2, 1.0f, false);
    recorder.setOutputDirectory("test_output");
    EXPECT_NO_THROW(recorder.start());
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_NO_THROW(recorder.stop());
    fs::remove_all("test_output");
}

TEST(FileRecorder, WriteWhileStoppedDoesNothing) {
    FileRecorder recorder(44100, 2, 1.0f, false);
    float buffer[8] = {0};
    EXPECT_NO_THROW(recorder.write(buffer, 4));
}

TEST(FileRecorder, StartTwiceIsSafe) {
    fs::create_directory("test_output");
    FileRecorder recorder(44100, 2, 1.0f, false);
    recorder.setOutputDirectory("test_output");
    recorder.start();
    EXPECT_NO_THROW(recorder.start());
    recorder.stop();
    fs::remove_all("test_output");
}
