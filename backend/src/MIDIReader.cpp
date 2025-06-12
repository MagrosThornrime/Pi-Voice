#include <MIDIReader.hpp>
#include <ranges>
#include <napi.h>
#include <chrono>

using namespace std::chrono_literals;

MIDIReader::MIDIReader() noexcept {
    setupGetter();
}

void MIDIReader::setupGetter() noexcept {
    getterThread = std::jthread([this](std::stop_token stopToken) {
        RtMidiIn midiIn;
        if (midiIn.getPortCount() == 0) {
            auto lock = std::lock_guard(mutex);
            messages.emplace("No devices found");
            return;
        }
        else {
            auto lock = std::lock_guard(mutex);
            for(auto i = 0u; i != midiIn.getPortCount(); ++i) {
                messages.emplace(std::format("MIDI device {}: '{}'", i, midiIn.getPortName(i)));
            }
        }

        midiIn.openPort();
        midiIn.ignoreTypes(false, false, false);

        for (; not stopToken.stop_requested(); std::this_thread::sleep_for(10ms)) {
            std::vector<unsigned char> msg;
            
            double stamp = midiIn.getMessage(&msg);
            if (msg.size() == 0) {
                continue;
            }
            
            std::string out;
            out += std::format("t = {}", stamp);
            for (size_t i = 0; i < msg.size(); ++i) {
                out += std::format("; msg[{}] = {}", i, int(msg[i]));
            }
            
            {
                auto lock = std::lock_guard(mutex);
                messages.emplace(std::move(out));
            }
        }
    });
}

std::unique_ptr<MIDIReader> MIDIReader::singleton;
MIDIReader& MIDIReader::get() noexcept {
    if(not singleton) singleton = std::make_unique<MIDIReader>();
    return *singleton;
}

void MIDIReader::cleanup() noexcept {
    get().getterThread.request_stop();
    get().getterThread.join();
    singleton.reset();
}

Napi::Array getMIDI(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    auto lock = std::lock_guard(MIDIReader::get().mutex);

    if(not MIDIReader::get().hasMIDI) {
        auto result = Napi::Array::New(env);
        result.Set(result.Length(), Napi::String::New(env, "<NO-DEVICE>"));
        return result;
    }

    auto result = Napi::Array::New(env, MIDIReader::get().messages.size());
    for (auto i = 0u; not MIDIReader::get().messages.empty(); ++i) {
        result[i] = Napi::String::New(env, MIDIReader::get().messages.front());
        MIDIReader::get().messages.pop();
    }
    return result;
}

void cleanup(const Napi::CallbackInfo& info) noexcept {
    MIDIReader::cleanup();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("getMIDI", Napi::Function::New(env, getMIDI));
    exports.Set("cleanup", Napi::Function::New(env, cleanup));
    return exports;
}
NODE_API_MODULE(MIDIReader, Init);