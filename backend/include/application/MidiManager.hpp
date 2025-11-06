#pragma once

#include <portaudiocpp/PortAudioCpp.hxx>
#include <application/Synthesiser.hpp>
#include <Midi.hpp>
#include <fmt/core.h>
#include <ranges>
#include <thread>
#include <mutex>
#include <memory>

using namespace std::chrono_literals;

namespace application {
class MidiManager {

    std::shared_ptr<Synthesiser> _synthesiser;
    midi::Reader _midiReader;
    std::unique_ptr<std::jthread> _thread;
    std::mutex _mutex;

    void _midiHandler(const midi::Data data);

public:
    MidiManager(std::shared_ptr<Synthesiser> synthesiser);
    void openMidiPort(i32 index);
    std::vector<midi::Port> listMidiPorts();

};

}