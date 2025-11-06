#pragma once
#include <polyphonic/VoiceManager.hpp>
#include <portaudiocpp/PortAudioCpp.hxx>
#include <fileio/FileRecorder.hpp>
#include <thread>
#include <Types.hpp>
#include <string>
#include <pipeline/Pipeline.hpp>

namespace application {
class Synthesiser {
    std::shared_ptr<pipeline::Pipeline> _pipeline;
    std::shared_ptr<fileio::FileRecorder> _recorder;
    std::unique_ptr<portaudio::InterfaceCallbackStream> _stream;
    std::shared_ptr<polyphonic::VoiceManager> _voiceManager;
    std::unique_ptr<portaudio::AutoSystem> _autoSys;

    std::mutex _mutex;

    i32 _sampleRate;
    i32 _channels;

    bool _running = false;

public:
    bool isRunning();
    Synthesiser(const std::string& recordingPath, i32 channels, i32 sampleRate);
    void pressKey(i32 key);
    void releaseKey(i32 key);
    void start();
    void stop();

    void setAmplitude(f32 amplitude);
    void setOscillatorType(oscillators::OscillatorType type, i32 index);
    void setOscillatorAmplitude(f32 amplitude, i32 index);
    void setAttack(f32 attack);
    void setDecay(f32 decay);
    void setSustain(f32 sustain);
    void setRelease(f32 release);
};

}