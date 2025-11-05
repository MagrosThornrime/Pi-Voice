#pragma once

#include <napi.h>
#include <portaudiocpp/PortAudioCpp.hxx>
#include <Midi.hpp>
#include <polyphonic/VoiceManager.hpp>
#include <pipeline/Pipeline.hpp>
#include <Filters.hpp>
#include <fileio/FileRecorder.hpp>
#include <fmt/core.h>
#include <ranges>
#include <thread>
#include <mutex>
#include <memory>
#include <atomic>

using namespace std::chrono_literals;

namespace application {
class App {

    std::shared_ptr<Synthesiser> _synthesiser;
    midi::Reader _midiReader;
    std::unique_ptr<std::jthread> _midiThread;
    std::mutex _midiMutex;

    void _midiHandler(const midi::Data data);

public:
    App(std::string recordingPath, i32 channels, i32 sampleRate);
    void openMidiPort(i32 index);
    void startSynthesiser();
    void stopSynthesiser();

};

}