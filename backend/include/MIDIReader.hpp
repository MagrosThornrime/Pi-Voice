#pragma once

#include <RtMidi.h>
#include <mutex>
#include <thread>
#include <queue>
#include <memory>

struct MIDIReader {
    std::mutex mutex;
    std::jthread getterThread;
    std::queue<std::string> messages;
    bool hasMIDI = true;

    MIDIReader() noexcept;

    void setupGetter() noexcept;
    
    static std::unique_ptr<MIDIReader> singleton;
    static MIDIReader& get() noexcept;
    static void cleanup() noexcept;
};