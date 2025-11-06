#include <application/MidiManager.hpp>
#include <application/Synthesiser.hpp>
#include <napi.h>
#include <Types.hpp>


std::shared_ptr<application::Synthesiser> synthesiser;
std::shared_ptr<application::MidiManager> midiApp;
std::mutex mutex;

void initializeApplication(){
    synthesiser = std::make_shared<application::Synthesiser>("capture_in.wav", 2, 44100);
    midiApp = std::make_shared<application::MidiManager>(synthesiser);
    synthesiser->start();
}

void destroyApplication(void*) {
    synthesiser->stop();
    midiApp.reset();
    synthesiser.reset();
}

Napi::Array getMidiPorts(const Napi::CallbackInfo& info) {
    auto lock = std::lock_guard(mutex);
    auto env = info.Env();
    auto ports = midiApp->listMidiPorts();
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

void openMidiPort(const Napi::CallbackInfo& info) {
    auto lock = std::lock_guard(mutex);
    auto env = info.Env();
    if (info.Length() != 1) {
        Napi::TypeError::New(env, "Expected port number").ThrowAsJavaScriptException();
        return;
    }
    i32 portNum = info[0].As<Napi::Number>().Int32Value();
    try {
        midiApp->openMidiPort(portNum);
        fmt::println("MIDI reader opened on port {}", portNum);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    }
}

void setAmplitude(const Napi::CallbackInfo& info) {
    auto lock = std::lock_guard(mutex);
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected 1 numeric argument (amplitude)").ThrowAsJavaScriptException();
        return;
    }

    f32 amp = info[0].As<Napi::Number>().FloatValue();
    amp = std::clamp(amp, 0.0f, 1.0f);
    synthesiser->setAmplitude(amp);
    fmt::println("Amplitude set to {}", amp);
}

// Set oscillator type (0,1,2)
void setOscillatorType(const Napi::CallbackInfo& info) {
    auto lock = std::lock_guard(mutex);
    auto env = info.Env();
    if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Expected (index:i32, type:i32)").ThrowAsJavaScriptException();
        return;
    }

    i32 index = info[1].As<Napi::Number>().Int32Value();
    std::string typeName = info[0].As<Napi::String>();
    if (index < 0 || index > 2) {
        Napi::RangeError::New(env, "Invalid oscillator index").ThrowAsJavaScriptException();
        return;
    }
    auto type = oscillators::oscillatorFromString(typeName);
    synthesiser->setOscillatorType(type, index);
    fmt::println("Oscillator {} type set to {}", index, typeName);
}

// Set oscillator amplitude (0,1,2)
void setOscillatorAmplitude(const Napi::CallbackInfo& info) {
    auto lock = std::lock_guard(mutex);
    auto env = info.Env();
    if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Expected (amplitude:f32, index:i32)").ThrowAsJavaScriptException();
        return;
    }
    i32 index = info[1].As<Napi::Number>().Int32Value();
    f32 amp = info[0].As<Napi::Number>().FloatValue();
    if (index < 0 || index > 2) {
        Napi::RangeError::New(env, "Invalid oscillator index").ThrowAsJavaScriptException();
        return;
    }
    amp = std::clamp(amp, 0.0f, 1.0f);
    synthesiser->setOscillatorAmplitude(amp, index);
    fmt::println("Oscillator {} amplitude set to {}", index, amp);
}

// ADSR controls
void setAttack(const Napi::CallbackInfo& info) {\
    auto lock = std::lock_guard(mutex);
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected attack:f32").ThrowAsJavaScriptException();
        return;
    }
	f32 attack = info[0].As<Napi::Number>().FloatValue();
    synthesiser->setAttack(attack);
    fmt::println("Attack set to {}", attack);
}

void setDecay(const Napi::CallbackInfo& info) {
    auto lock = std::lock_guard(mutex);
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected decay:f32").ThrowAsJavaScriptException();
        return;
    }
	f32 decay = info[0].As<Napi::Number>().FloatValue();
    synthesiser->setDecay(decay);
	fmt::println("Decay set to {}", decay);
}

void setSustain(const Napi::CallbackInfo& info) {
    auto lock = std::lock_guard(mutex);
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected sustain:f32").ThrowAsJavaScriptException();
        return;
    }
	f32 sustain = info[0].As<Napi::Number>().FloatValue();
    synthesiser->setSustain(sustain);
	fmt::println("Sustain set to {}", sustain);
}

void setRelease(const Napi::CallbackInfo& info) {
    auto lock = std::lock_guard(mutex);
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected release:f32").ThrowAsJavaScriptException();
        return;
    }
	f32 release = info[0].As<Napi::Number>().FloatValue();
    synthesiser->setRelease(release);
	fmt::println("Release set to {}", release);
}


Napi::Object Init(Napi::Env env, Napi::Object exports) {
    initializeApplication();

    exports.Set("midiPorts", Napi::Function::New(env, getMidiPorts));
    exports.Set("openMidi", Napi::Function::New(env, openMidiPort));

    exports.Set("setAmplitude", Napi::Function::New(env, setAmplitude));
    exports.Set("setOscillatorType", Napi::Function::New(env, setOscillatorType));
    exports.Set("setOscillatorAmplitude", Napi::Function::New(env, setOscillatorAmplitude));
    exports.Set("setAttack", Napi::Function::New(env, setAttack));
    exports.Set("setDecay", Napi::Function::New(env, setDecay));
    exports.Set("setSustain", Napi::Function::New(env, setSustain));
    exports.Set("setRelease", Napi::Function::New(env, setRelease));

    env.AddCleanupHook(destroyApplication, (void*)nullptr);

    fmt::println("SynthModule initialized successfully");
    return exports;
}

NODE_API_MODULE(SynthModule, Init);