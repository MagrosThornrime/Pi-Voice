#pragma once

#include <portaudiocpp/PortAudioCpp.hxx>
#include <application/Synthesiser.hpp>
#include <Midi.hpp>
#include <fmt/core.h>
#include <ranges>
#include <thread>
#include <mutex>
#include <memory>
#include <chrono>

using namespace std::chrono_literals;

namespace application {

/// @brief Reads input from midi keyboard and passes it to the synthesiser
class MidiManager {

    std::shared_ptr<Synthesiser> _synthesiser;
    midi::Reader _midiReader;
    std::mutex _mutex;

    /// @brief Passes midi input to the synthesiser
    /// @param data midi input from reader
    void _midiHandler(const midi::Data data);

public:
    /// @brief Constructor
    /// @param synthesiser High level API for sound synthesis
    MidiManager(std::shared_ptr<Synthesiser> synthesiser);

    /// @brief Starts listening to the particular port (you can listen to only one at once)
    /// @param index id of the port
    void openMidiPort(i32 index);

    /// @brief Gets all available midi ports
    std::vector<midi::Port> listMidiPorts();

};

}