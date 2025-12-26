#pragma once

#include <application/MidiManager.hpp>
#include <application/Synthesiser.hpp>
#include <napi.h>
#include <Types.hpp>

extern std::shared_ptr<application::Synthesiser> synthesiser;
extern std::shared_ptr<application::MidiManager> midiApp;
extern std::mutex mutex;