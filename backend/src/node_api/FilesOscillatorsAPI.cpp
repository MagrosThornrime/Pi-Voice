#include <node_api/FilesOscillatorsAPI.hpp>
#include <BackendApp.hpp>

namespace node_api{

void initFiles(Napi::Env env, Napi::Object exports){
	exports.Set("startRecording", Napi::Function::New(env, startRecording));
	exports.Set("stopRecording", Napi::Function::New(env, stopRecording));
	exports.Set("setRecordingPath", Napi::Function::New(env, setRecordingPath));
	exports.Set("setSamplesPath", Napi::Function::New(env, setSamplesPath));
	exports.Set("getOscillatorsNames", Napi::Function::New(env, getOscillatorNames));
	exports.Set("getOscillatorPlot", Napi::Function::New(env, getOscillatorPlot));
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
}