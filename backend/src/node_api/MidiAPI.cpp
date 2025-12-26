#include <node_api/MidiAPI.hpp>
#include <BackendApp.hpp>
#include <range/v3/all.hpp>

namespace node_api{

void initMidi(Napi::Env env, Napi::Object exports){
    exports.Set("midiPorts", Napi::Function::New(env, getMidiPorts));
    exports.Set("openMidi", Napi::Function::New(env, openMidiPort));
}

Napi::Array getMidiPorts(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    auto lock = std::lock_guard(mutex);
    auto ports = midiApp->listMidiPorts();
    auto result = Napi::Array::New(env);

    if (ports.empty()) {
        result.Set(0u, Napi::String::New(env, "NO DEVICES"));
        return result;
    }

    for (auto&& [i, port] : ports | ranges::views::enumerate) {
        result.Set(i, Napi::String::New(env, fmt::format("{}: '{}'", port.num, port.name)));
    }

    return result;
}

void openMidiPort(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 1) {
        Napi::TypeError::New(env, "Expected port number").ThrowAsJavaScriptException();
        return;
    }
    i32 portNum = info[0].As<Napi::Number>().Int32Value();
    try {
        auto lock = std::lock_guard(mutex);
        midiApp->openMidiPort(portNum);
        fmt::println("MIDI reader opened on port {}", portNum);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    }
}
}