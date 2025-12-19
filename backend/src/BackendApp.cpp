#include <application/MidiManager.hpp>
#include <application/Synthesiser.hpp>
#include <napi.h>
#include <Types.hpp>
#include <application/BackendApp.hpp>
#include <application/PipelineAPI.hpp>
#include <range/v3/all.hpp>

std::shared_ptr<application::Synthesiser> synthesiser{};
std::shared_ptr<application::MidiManager> midiApp{};
std::mutex mutex{};

void initializeApplication(){
    synthesiser = std::make_shared<application::Synthesiser>("capture_in.wav", 2, 44100, "res/samples");
    midiApp = std::make_shared<application::MidiManager>(synthesiser);
    synthesiser->start();
}

void destroyApplication(void*) {
	synthesiser->stop();
	midiApp.reset();
	synthesiser.reset();
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

void setAmplitude(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected 1 numeric argument (amplitude)").ThrowAsJavaScriptException();
        return;
    }

	f32 amp = info[0].As<Napi::Number>().FloatValue();
	try {
    	auto lock = std::lock_guard(mutex);
    	synthesiser->setAmplitude(amp);
	} catch (const std::exception& e) {
		Napi::RangeError::New(env, e.what()).ThrowAsJavaScriptException();
		return;
	}
	fmt::println("Amplitude set to {}", amp);
}

// Set oscillator type (0,1,2)
void setOscillatorType(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 2 || !info[0].IsString() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Expected (type:string, index:i32)").ThrowAsJavaScriptException();
        return;
    }
    std::string type = info[0].As<Napi::String>();
    i32 index = info[1].As<Napi::Number>().Int32Value();
    if (index < 0 || index > 2) {
        Napi::RangeError::New(env, "Invalid oscillator index").ThrowAsJavaScriptException();
        return;
    }
	try {
    	auto lock = std::lock_guard(mutex);
    	synthesiser->setOscillatorType(type, index);
	} catch (const std::exception& e) {
		Napi::RangeError::New(env, e.what()).ThrowAsJavaScriptException();
		return;
	}
    fmt::println("Oscillator {} type set to {}", index, type);
}


// ADSR controls
void setAttack(const Napi::CallbackInfo& info) {\
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected attack:f32").ThrowAsJavaScriptException();
        return;
    }
	f32 attack = info[0].As<Napi::Number>().FloatValue();
	try {
    	auto lock = std::lock_guard(mutex);
    	synthesiser->setAttack(attack);
	} catch (const std::exception& e) {
		Napi::RangeError::New(env, e.what()).ThrowAsJavaScriptException();
		return;
	}
	fmt::println("Attack set to {}", attack);
}

void setDecay(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected decay:f32").ThrowAsJavaScriptException();
        return;
    }
	f32 decay = info[0].As<Napi::Number>().FloatValue();
	try {
		auto lock = std::lock_guard(mutex);
    	synthesiser->setDecay(decay);
	} catch (const std::exception& e) {
		Napi::RangeError::New(env, e.what()).ThrowAsJavaScriptException();
		return;
	}
	fmt::println("Decay set to {}", decay);
}

void setSustain(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected sustain:f32").ThrowAsJavaScriptException();
        return;
    }
	f32 sustain = info[0].As<Napi::Number>().FloatValue();
	try {
    	auto lock = std::lock_guard(mutex);
    	synthesiser->setSustain(sustain);
	} catch (const std::exception& e) {
		Napi::RangeError::New(env, e.what()).ThrowAsJavaScriptException();
		return;
	}
	fmt::println("Sustain set to {}", sustain);
}

void setRelease(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected release:f32").ThrowAsJavaScriptException();
        return;
    }
	f32 release = info[0].As<Napi::Number>().FloatValue();
	try {
    	auto lock = std::lock_guard(mutex);
    	synthesiser->setRelease(release);
	} catch (const std::exception& e) {
		Napi::RangeError::New(env, e.what()).ThrowAsJavaScriptException();
		return;
	}
	fmt::println("Release set to {}", release);
}

// Recorder
void startRecording(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 0) {
        Napi::TypeError::New(env, "Expected no arguments").ThrowAsJavaScriptException();
        return;
    }
    auto lock = std::lock_guard(mutex);
    synthesiser->startRecording();
	fmt::println("Started recording");
}

void stopRecording(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 0) {
        Napi::TypeError::New(env, "Expected no arguments").ThrowAsJavaScriptException();
        return;
    }
    auto lock = std::lock_guard(mutex);
    synthesiser->stopRecording();
	fmt::println("Stopped recording");
}

void setRecordingPath(const Napi::CallbackInfo& info) {
	auto env = info.Env();
	if (info.Length() != 1 || !info[0].IsString()) {
		Napi::TypeError::New(env, "Expected type:string").ThrowAsJavaScriptException();
		return;
	}
	std::string path = info[0].As<Napi::String>().ToString();
	try {
		auto lock = std::lock_guard(mutex);
		synthesiser->setRecordingPath(path);
	} catch (const std::exception& e) {
		Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
		return;
	}
	fmt::println("Recording path set to {}", path);
}

void setSamplesPath(const Napi::CallbackInfo& info) {
	auto env = info.Env();
    if (info.Length() != 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected type:string").ThrowAsJavaScriptException();
        return;
    }
	std::string path = info[0].As<Napi::String>().ToString();
	try {
		auto lock = std::lock_guard(mutex);
		synthesiser->setSamplesPath(path);
	}
	catch (const std::exception& e) {
		Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
		return;
	}
	fmt::println("Samples path set to {}", path);
}

Napi::Array getOscillatorNames(const Napi::CallbackInfo& info) {
	auto env = info.Env();
	auto lock = std::lock_guard(mutex);
	auto names = synthesiser->getSampleNames();
	Napi::Array result = Napi::Array::New(env, names.size());
	if (names.empty()) {
		Napi::Error::New(env, "No oscillators found").ThrowAsJavaScriptException();
		return result;
	}
	for (i32 i = 0; i < names.size(); i++) {
		result.Set(i, Napi::String::New(env, names[i]));
	}
	return result;
}


Napi::Array getOscillatorPlot(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() != 2 || !info[0].IsString() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Expected (type:string, length:i32)")
            .ThrowAsJavaScriptException();
        return Napi::Array::New(env);
    }
    std::string name = info[0].As<Napi::String>();
    i32 length = info[1].As<Napi::Number>().Int32Value();
    std::vector<f32> plot;
    try {
    	auto lock = std::lock_guard(mutex);
        plot = synthesiser->getOscillatorPlot(name, length);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return Napi::Array::New(env);
    }
    Napi::Array result = Napi::Array::New(env, plot.size());
    for (i32 i = 0; i < plot.size(); i++) {
        result.Set(i, Napi::Number::New(env, plot[i]));
    }
    return result;
}


Napi::Object Init(Napi::Env env, Napi::Object exports) {
	initializeApplication();

	exports.Set("midiPorts", Napi::Function::New(env, getMidiPorts));
	exports.Set("openMidi", Napi::Function::New(env, openMidiPort));

	exports.Set("setAmplitude", Napi::Function::New(env, setAmplitude));
	exports.Set("setOscillatorType", Napi::Function::New(env, setOscillatorType));
	exports.Set("setAttack", Napi::Function::New(env, setAttack));
	exports.Set("setDecay", Napi::Function::New(env, setDecay));
	exports.Set("setSustain", Napi::Function::New(env, setSustain));
	exports.Set("setRelease", Napi::Function::New(env, setRelease));

	exports.Set("startRecording", Napi::Function::New(env, startRecording));
	exports.Set("stopRecording", Napi::Function::New(env, stopRecording));
	exports.Set("setRecordingPath", Napi::Function::New(env, setRecordingPath));

	pipelineAPI::init(env, exports);
	
	exports.Set("setSamplesPath", Napi::Function::New(env, setSamplesPath));
	exports.Set("getOscillatorsNames", Napi::Function::New(env, getOscillatorNames));
	exports.Set("getOscillatorPlot", Napi::Function::New(env, getOscillatorPlot));

    env.AddCleanupHook(destroyApplication, (void*)nullptr);

	fmt::println("SynthModule initialized successfully");
	return exports;
}

NODE_API_MODULE(SynthModule, Init);