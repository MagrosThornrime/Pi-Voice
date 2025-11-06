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

std::shared_ptr<polyphonic::VoiceManager> g_voiceManager;
std::shared_ptr<fileio::FileRecorder> g_recorder;
std::unique_ptr<portaudio::InterfaceCallbackStream> g_stream;
std::unique_ptr<std::jthread> g_midiThread;
auto mypipe = pipeline::Pipeline();

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
        g_voiceManager = std::make_shared<polyphonic::VoiceManager>(128, 44100.0f);
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
        mypipe.setSource(g_voiceManager)
                .addLayer(std::make_shared<LowPassFilter>(2, 2, 1000, 44100))
                .addLayer(g_recorder);

        g_stream = std::make_unique<portaudio::InterfaceCallbackStream>(streamParams, mypipe);
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
// Synth Parameter Controls
// ========================

// Set global amplitude
void setAmplitude(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected 1 numeric argument (amplitude)").ThrowAsJavaScriptException();
        return;
    }

    if (!g_voiceManager) return;

    float amp = info[0].As<Napi::Number>().FloatValue();
    amp = std::clamp(amp, 0.0f, 1.0f);
    g_voiceManager->setAmplitude(amp);
    fmt::println("Amplitude set to {}", amp);
}

// Set oscillator type (0,1,2)
void setOscillatorType(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Expected (index:int, type:int)").ThrowAsJavaScriptException();
        return;
    }

    if (!g_voiceManager) return;

    int index = info[0].As<Napi::Number>().Int32Value();
    int typeVal = info[1].As<Napi::Number>().Int32Value();
    if (index < 0 || index > 2 || typeVal < 0 || typeVal > oscillators::empty) {
        Napi::RangeError::New(env, "Invalid oscillator index or type").ThrowAsJavaScriptException();
        return;
    }

    g_voiceManager->setOscillatorType(static_cast<oscillators::OscillatorType>(typeVal), index);
    fmt::println("Oscillator {} type set to {}", index, typeVal);
}

// Set oscillator amplitude (0,1,2)
void setOscillatorAmplitude(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Expected (index:int, amplitude:float)").ThrowAsJavaScriptException();
        return;
    }

    if (!g_voiceManager) return;

    int index = info[0].As<Napi::Number>().Int32Value();
    float amp = info[1].As<Napi::Number>().FloatValue();
    if (index < 0 || index > 2) {
        Napi::RangeError::New(env, "Invalid oscillator index").ThrowAsJavaScriptException();
        return;
    }

    amp = std::clamp(amp, 0.0f, 1.0f);
    g_voiceManager->setOscillatorAmplitude(amp, index);
    fmt::println("Oscillator {} amplitude set to {}", index, amp);
}

// ADSR controls
void setAttack(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected attack:float").ThrowAsJavaScriptException();
        return;
    }
    if (g_voiceManager) g_voiceManager->setAttack(info[0].As<Napi::Number>().FloatValue());
}

void setDecay(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected decay:float").ThrowAsJavaScriptException();
        return;
    }
    if (g_voiceManager) g_voiceManager->setDecay(info[0].As<Napi::Number>().FloatValue());
}

void setSustain(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected sustain:float").ThrowAsJavaScriptException();
        return;
    }
    if (g_voiceManager) g_voiceManager->setSustain(info[0].As<Napi::Number>().FloatValue());
}

void setRelease(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected release:float").ThrowAsJavaScriptException();
        return;
    }
    if (g_voiceManager) g_voiceManager->setRelease(info[0].As<Napi::Number>().FloatValue());
}

// ========================
// Filter Controls
// ========================

// ========================
// Filter Controls (fixed)
// ========================
void addFilter(const Napi::CallbackInfo& info) {
    auto env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected filter name (string)").ThrowAsJavaScriptException();
        return;
    }

    std::string name = info[0].As<Napi::String>();
    fmt::println("Adding filter: {}", name);

    // Construct the appropriate filter instance
    std::shared_ptr<portaudio::CallbackInterface> filter;

    if (name == "lowpass") {
        filter = std::make_shared<LowPassFilter>(2, 2, 1000, 44100);
    } else if (name == "highpass") {
        filter = std::make_shared<HighPassFilter>(2, 2, 1000, 44100);
    } else if (name == "bandpass") {
        filter = std::make_shared<BandPassFilter>(2, 2, 1000, 44100);
    } else if (name == "notch") {
        filter = std::make_shared<NotchFilter>(2, 2, 1000, 44100);
    } else if (name == "lowshelf") {
        filter = std::make_shared<LowShelfFilter>(2, 2, 1000, 44100);
    } else if (name == "highshelf") {
        filter = std::make_shared<HighShelfFilter>(2, 2, 1000, 44100);
    } else if (name == "peaking") {
        filter = std::make_shared<PeakingEQFilter>(2, 2, 1000, 44100);
    } else if (name == "allpass") {
        filter = std::make_shared<AllPassFilter>(2, 2, 1000, 44100);
    } else {
        Napi::Error::New(env, "Unknown filter name").ThrowAsJavaScriptException();
        return;
    }

    mypipe.addLayer(filter);
    fmt::println("Filter '{}' added successfully", name);
}

// Remove all filters except recorder
void clearFilters(const Napi::CallbackInfo& info) {
    // Preserve source and recorder
    auto vm = g_voiceManager;
    auto rec = g_recorder;

    // Rebuild pipeline cleanly
    pipeline::Pipeline newPipe;
    if (vm) newPipe.setSource(vm);
    if (rec) newPipe.addLayer(rec);

    mypipe = std::move(newPipe);

    fmt::println("All filters cleared and pipeline rebuilt");
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

    exports.Set("setAmplitude", Napi::Function::New(env, setAmplitude));
    exports.Set("setOscillatorType", Napi::Function::New(env, setOscillatorType));
    exports.Set("setOscillatorAmplitude", Napi::Function::New(env, setOscillatorAmplitude));
    exports.Set("setAttack", Napi::Function::New(env, setAttack));
    exports.Set("setDecay", Napi::Function::New(env, setDecay));
    exports.Set("setSustain", Napi::Function::New(env, setSustain));
    exports.Set("setRelease", Napi::Function::New(env, setRelease));
    exports.Set("addFilter", Napi::Function::New(env, addFilter));
    exports.Set("clearFilters", Napi::Function::New(env, clearFilters));

    fmt::println("SynthModule initialized successfully");
    return exports;
}

NODE_API_MODULE(SynthModule, Init);