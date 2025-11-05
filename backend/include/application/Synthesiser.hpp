#pragma once
#include <polyphonic/VoiceManager.hpp>
#include <portaudiocpp/PortAudioCpp.hxx>
#include <fileio/FileRecorder.hpp>
#include <thread>
#include <Types.hpp>
#include <string>

namespace application {
class Synthesiser {
    polyphonic::VoiceManager _voiceManager;
    std::shared_ptr<Pipeline> _pipeline;
    std::shared_ptr<fileio::FileRecorder> _recorder;
    std::unique_ptr<portaidop::InterfaceCallbackStream> _stream;

    bool running = false;

public:
    bool isRunning();
    Synthesiser(std::string recordingPath, i32 channels, i32 sampleRate);
    void pressKey(i32 key);
    void releaseKey(i32 key);
    void start();
    void stop();
};

}