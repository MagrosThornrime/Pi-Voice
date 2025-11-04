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

// ========================
// Global Resources
// ========================

// Make AutoSystem static to persist across addon lifetime
std::unique_ptr<portaudio::AutoSystem> g_autoSys;

void ensurePortAudio() {
    if (!g_autoSys) {
        g_autoSys = std::make_unique<portaudio::AutoSystem>();
        fmt::println("PortAudio system initialized");
    }
}

std::shared_ptr<VoiceManager> g_voiceManager;
std::shared_ptr<fileio::FileRecorder> g_recorder;
std::unique_ptr<portaudio::InterfaceCallbackStream> g_stream;
std::unique_ptr<std::jthread> g_midiThread;

midi::Reader g_reader;
std::mutex g_midiMutex;
std::atomic<bool> g_running{false};

// ========================
// MIDI Callback
// ========================
void midiHandler(const midi::Data data) noexcept {
    std::scoped_lock lock(g_midiMutex);
    auto vm = g_voiceManager;
    if (!vm) return;

    if (!data.hasNote()) return;

    if (data.status() == midi::Data::noteOn) {
        vm->turnOn(data.note().num);
    } else if (data.status() == midi::Data::noteOff) {
        vm->turnOff(data.note().num);
    }
}

// ========================
// JS Functions
// ========================

// List MIDI ports
Napi::Array midiPorts(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    auto ports = midi::Ports::locked(midi::Ports::list);
    auto result = Napi::Array::New(env);

    if (ports.empty()) {
        result.Set(0u, Napi::String::New(env, "NO DEVICES"));
        return result;
    }

    for (auto&& [i, port] : ports | std::views::enumerate) {
        result.Set(i, Napi::String::New(env, fmt::format("{}: '{}'", port.num, port.name)));
    }

    return result;
}

// Open MIDI port safely
void openMidi(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 1) {
        Napi::TypeError::New(env, "Expected port number").ThrowAsJavaScriptException();
        return;
    }

    int portNum = info[0].As<Napi::Number>().Int32Value();

    try {
        g_reader.open(midi::Ports::locked(midi::Ports::getByNum, portNum));
        g_reader.setGeneralCallback(midiHandler);
        fmt::println("MIDI reader opened on port {}", portNum);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    }
}

// Start Synth
void startSynth(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (g_running.load()) {
        fmt::println("Synth already running");
        return;
    }

    try {
    	ensurePortAudio();
        auto& sys = portaudio::System::instance();
        auto& outputDevice = sys.defaultOutputDevice();

        portaudio::DirectionSpecificStreamParameters outParams(
            outputDevice,
            2,
            portaudio::FLOAT32,
            true,
            outputDevice.defaultLowOutputLatency(),
            nullptr
        );

        portaudio::StreamParameters streamParams(
            portaudio::DirectionSpecificStreamParameters::null(),
            outParams,
            44100,
            256,
            paClipOff
        );

        // Setup VoiceManager
        g_voiceManager = std::make_shared<VoiceManager>(128, 44100.0f);
        g_voiceManager->setOscillatorType(oscillators::square, 0);
        g_voiceManager->setOscillatorType(oscillators::triangle, 1);
        g_voiceManager->setOscillatorType(oscillators::sawtooth, 2);
        g_voiceManager->setAttack(0.0004f);
        g_voiceManager->setDecay(0.0001f);
        g_voiceManager->setSustain(0.5f);
        g_voiceManager->setRelease(0.0001f);

        // Setup Recorder
        g_recorder = std::make_shared<fileio::FileRecorder>("capture_in.wav", 2, 44100);
        if (!g_recorder->start()) {
            Napi::Error::New(env, "Failed to start recorder").ThrowAsJavaScriptException();
            return;
        }

        // Setup Pipeline
        auto pipeline = Pipeline();
        pipeline.setSource(g_voiceManager)
                .addLayer(std::make_shared<LowPassFilter>(2, 2, 1000, 44100))
                .addLayer(g_recorder);

        g_stream = std::make_unique<portaudio::InterfaceCallbackStream>(streamParams, pipeline);
        g_stream->start();

        g_running.store(true);
        fmt::println("Audio stream started");

    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    }
}

// Stop Synth
void stopSynth(const Napi::CallbackInfo& info) {
    if (!g_running.load()) return;

    try {
        if (g_recorder) g_recorder->stop();
        if (g_stream) {
            g_stream->stop();
            g_stream->close();
            g_stream.reset();
        }

        // Stop MIDI safely
        std::scoped_lock lock(g_midiMutex);
        g_voiceManager.reset();
        g_recorder.reset();
        g_reader.close();

        g_running.store(false);
        fmt::println("Synth stopped");

    } catch (const std::exception& e) {
        fmt::println("Stop error: {}", e.what());
    }
}

// Cleanup
void cleanup(const Napi::CallbackInfo& info) {
    stopSynth(info);
    fmt::println("Cleaned up");
}

// ========================
// Module Init
// ========================
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("midiPorts", Napi::Function::New(env, midiPorts));
    exports.Set("openMidi", Napi::Function::New(env, openMidi));
    exports.Set("startSynth", Napi::Function::New(env, startSynth));
    exports.Set("stopSynth", Napi::Function::New(env, stopSynth));
    exports.Set("cleanup", Napi::Function::New(env, cleanup));

    fmt::println("SynthModule initialized successfully");
    return exports;
}

NODE_API_MODULE(SynthModule, Init);
