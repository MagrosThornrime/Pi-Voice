#include <node_api/VoiceManagerAPI.hpp>
#include <BackendApp.hpp>

namespace node_api {

void initVoiceManager(Napi::Env env, Napi::Object exports) {
	exports.Set("setAmplitude", Napi::Function::New(env, setAmplitude));
	exports.Set("setOscillatorType", Napi::Function::New(env, setOscillatorType));
	exports.Set("setAttack", Napi::Function::New(env, setAttack));
	exports.Set("setDecay", Napi::Function::New(env, setDecay));
	exports.Set("setSustain", Napi::Function::New(env, setSustain));
	exports.Set("setRelease", Napi::Function::New(env, setRelease));
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
}