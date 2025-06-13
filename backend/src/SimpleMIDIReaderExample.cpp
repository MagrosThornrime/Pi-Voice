#include <Midi.hpp>
#include <thread>
#include <chrono>
#include <napi.h>
#include <fmt/core.h>
#include <range/v3/all.hpp>

using namespace std::chrono_literals;
using namespace fmt::literals;

midi::Reader reader;
std::mutex dataMutex;
std::vector<midi::Data> data; 

void openReader(const Napi::CallbackInfo& info) noexcept;
void midiCallback(midi::InternalReader& reader) noexcept;
Napi::Array readMidi(const Napi::CallbackInfo& info) noexcept;

// std::jthread thread;
// void stopWorkerThread();
// void setupWorkerThread();
// void midiWorker(std::stop_token stopToken) noexcept;

void openReader(const Napi::CallbackInfo& info) noexcept {
    auto env = info.Env();
    if(info.Length() != 1) {
        Napi::Error::New(env, fmt::format("openReader: expected 1 argument, got {}", info.Length()));
    }
    u32 n;
    try {
        n = info[0].As<Napi::Number>().Int32Value();
    }
    catch (Napi::Error& e) {
        e.ThrowAsJavaScriptException();
        return;
    }

    try {
        auto&& [_, reader] = ::reader.locked();

        if(reader.opened()) reader.close();

        reader.open(midi::Ports::locked(midi::Ports::getByNum, n));
		reader.setCallback(midiCallback);
    }
    catch (std::exception& e) {
        reader.close();
		fmt::println("{}", e.what());

        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    }
}

void midiCallback(midi::InternalReader& reader) noexcept {
	auto lock = std::lock_guard(dataMutex);
	data.push_back(reader.read());
}

Napi::Array readMidi(const Napi::CallbackInfo& info) noexcept {
	auto env = info.Env();
	auto lock = std::lock_guard(dataMutex);

	std::vector<midi::Note> notes;
	for(auto&& d : data) {
		if(d.hasNote()) notes.push_back(d.note());
	}
	data.clear();

	auto result = Napi::Array::New(env);
	for(auto&& [i, note] : notes | ranges::views::enumerate) {
		result.Set(i, Napi::String::New(env, fmt::format("{}\t{}\t{}Hz", note.name, note.num, note.freq)));
	}

	return result;
}

Napi::Array midiPorts(const Napi::CallbackInfo& info) noexcept {
	auto env = info.Env();
	auto ports = midi::Ports::locked(midi::Ports::list);
	auto result = Napi::Array::New(env);
	if(ports.size() == 0) {
		result.Set(0u, Napi::String::New(env, "NO DEVICES"));
		return result;
	}

	for(auto&& [i, port] : ports | ranges::views::enumerate) {
		result.Set(i, Napi::String::New(env, fmt::format("{}: '{}'", port.num, port.name)));
	}
	return result;
}

void cleanup(const Napi::CallbackInfo& info) noexcept {
	reader.close();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("midiPorts", Napi::Function::New(env, midiPorts));
    exports.Set("openReader", Napi::Function::New(env, openReader));
    exports.Set("readMidi", Napi::Function::New(env, readMidi));
    exports.Set("cleanup", Napi::Function::New(env, cleanup));

    return exports;
}
NODE_API_MODULE(SimpleMIDIReaderExample, Init);

// void setupWorkerThread() {
//     stopWorkerThread();
//     thread = std::jthread(midiWorker);
// }

// void stopWorkerThread() {
//     if(thread.get_id() != std::thread::id()) {
//         thread.request_stop();
//         thread.join();
//     }
// }

// void midiWorker(std::stop_token stopToken) noexcept {
//     for(;; std::this_thread::sleep_for(10ms))
// }

// SimpleMIDIReaderExample::SimpleMIDIReaderExample() noexcept {
//     setupGetter();
// }

// void SimpleMIDIReaderExample::setupGetter() noexcept {
//     getterThread = std::jthread([this](std::stop_token stopToken) {
//         try {
//             reader.init(midi::Ports::get(0));
//         }
//         catch(std::exception& e) {

//         }
//     });

//     getterThread = std::jthread([this](std::stop_token stopToken) {
//         if (reader.getPortCount() == 0) {
//             auto lock = std::lock_guard(mutex);
//             messages.emplace("No devices found");
//             return;
//         }
//         else {
//             auto lock = std::lock_guard(mutex);
//             for(auto i = 0u; i != midiIn.getPortCount(); ++i) {
//                 messages.emplace(fmt::format("MIDI device {}: '{}'", i, midiIn.getPortName(i)));
//             }
//         }

//         midiIn.openPort();
//         midiIn.ignoreTypes(false, false, false);

//         for (; not stopToken.stop_requested(); std::this_thread::sleep_for(10ms)) {
//             std::vector<unsigned char> msg;
            
//             double stamp = midiIn.getMessage(&msg);
//             if (msg.size() == 0) {
//                 continue;
//             }
            
//             std::string out;
//             out += fmt::format("t = {}", stamp);
//             for (auto&& [i, byte] : ranges::views::enumerate(msg)) {
//                 out += fmt::format("; msg[{}] = {}", i, int(byte));
//             }
            
//             {
//                 auto lock = std::lock_guard(mutex);
//                 messages.emplace(std::move(out));
//             }
//         }
//     });
// }

// std::unique_ptr<SimpleMIDIReaderExample> SimpleMIDIReaderExample::singleton;
// SimpleMIDIReaderExample& SimpleMIDIReaderExample::get() {
//     if(not singleton) singleton = std::make_unique<SimpleMIDIReaderExample>();
//     return *singleton;
// }

// void SimpleMIDIReaderExample::cleanup() noexcept {
//     get().getterThread.request_stop();
//     get().getterThread.join();
//     singleton.reset();
// }

// Napi::Array getMIDI(const Napi::CallbackInfo& info) {
//     auto env = info.Env();

//     auto&& reader = 

//     auto lock = std::lock_guard(SimpleMIDIReaderExample::get().mutex);

//     if(not SimpleMIDIReaderExample::get()) {
//         auto result = Napi::Array::New(env);
//         result.Set(result.Length(), Napi::String::New(env, "<NO-DEVICE>"));
//         return result;
//     }

//     auto result = Napi::Array::New(env, SimpleMIDIReaderExample::get().messages.size());
//     for (auto i = 0u; not SimpleMIDIReaderExample::get().messages.empty(); ++i) {
//         result[i] = Napi::String::New(env, SimpleMIDIReaderExample::get().messages.front());
//         SimpleMIDIReaderExample::get().messages.pop();
//     }
//     return result;
// }