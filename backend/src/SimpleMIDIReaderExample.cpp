#include <Midi.hpp>
#include <thread>
#include <chrono>
#include <napi.h>
#include <fmt/core.h>
#include <range/v3/all.hpp>
#include <ranges>

using namespace std::chrono_literals;

// global midi reader
midi::Reader reader;
// extracted data
std::vector<midi::Data> data;
// mutex of extracted data
std::mutex dataMutex;

// JS function opening reader of given number
void openReader(const Napi::CallbackInfo& info) noexcept;
// callback to set in reader
void midiCallback(midi::InternalReader& reader) noexcept;
// JS function reading midi data (human readable)
Napi::Array readMidi(const Napi::CallbackInfo& info) noexcept;
// JS function reading available midi ports
Napi::Array midiPorts(const Napi::CallbackInfo& info) noexcept;
// JS cleanup callback
void cleanup(const Napi::CallbackInfo& info) noexcept;
// .node module hook
Napi::Object Init(Napi::Env env, Napi::Object exports);

void openReader(const Napi::CallbackInfo& info) noexcept {
	auto env = info.Env();
	if (info.Length() != 1) {
		Napi::Error::New(env, fmt::format("openReader: expected 1 argument, got {}", info.Length()));
	}

	u32 n;
	try {
		n = info[0].As<Napi::Number>().Int32Value();
	} catch (Napi::Error& e) {
		e.ThrowAsJavaScriptException();
		return;
	}

	try {
		// locking reader
		auto&& [_, reader] = ::reader.locked();

		// opening n-th port on reader
		reader.open(midi::Ports::locked(midi::Ports::getByNum, n));
		// setting midiCallback as event handler
		reader.setCallback(midiCallback);
	} catch (std::exception& e) {
		reader.close();
		fmt::println("{}", e.what());

		Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
	}
}

void midiCallback(midi::InternalReader& reader) noexcept {
	auto lock = std::lock_guard(dataMutex);
	// read event to data
	data.push_back(reader.read());
}

Napi::Array readMidi(const Napi::CallbackInfo& info) noexcept {
	auto env = info.Env();
	auto lock = std::lock_guard(dataMutex);

	auto notes = data
		| std::views::filter(std::mem_fn(&midi::Data::hasNote))
		| std::views::transform(std::mem_fn(&midi::Data::note))
		| ranges::to<std::vector>;
	data.clear();

	auto result = Napi::Array::New(env);
	for (auto&& [i, note] : notes | ranges::views::enumerate) {
		result.Set(i, Napi::String::New(env,
			// example: "C#_5 <num> <freq>"
			fmt::format("{}\t{}\t{}Hz", note.name, note.num, note.freq)));
	}

	return result;
}

Napi::Array midiPorts(const Napi::CallbackInfo& info) noexcept {
	auto env = info.Env();
	auto ports = midi::Ports::locked(midi::Ports::list);
	auto result = Napi::Array::New(env);
	if (ports.size() == 0) {
		result.Set(0u, Napi::String::New(env, "NO DEVICES"));
		return result;
	}

	for (auto&& [i, port] : ports | ranges::views::enumerate) {
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
