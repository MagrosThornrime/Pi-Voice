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

/// @brief Reads input from midi keyboard and passes it to the synthesiser
class MidiManager {

    /// @brief Plays sound
    std::shared_ptr<Synthesiser> _synthesiser;

    /// @brief Reads data from midi
    midi::Reader _midiReader;

    /// @brief Used for synchronization between changing ports and reading data
    std::mutex _mutex;

    /// @brief Passes midi input to the synthesiser
    /// @param data midi data
    void _midiHandler(const midi::Data data);

public:
    /// @brief Constructor
    /// @param synthesiser Plays sounds
    MidiManager(std::shared_ptr<Synthesiser> synthesiser);

    /// @brief Starts listening to the particular port (you can listen to only one at once)
    /// @param index id of the port
    void openMidiPort(i32 index);

    /// @brief Gets all available midi ports
    std::vector<midi::Port> listMidiPorts();

};

}