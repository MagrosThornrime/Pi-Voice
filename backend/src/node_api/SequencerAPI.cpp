#include <node_api/SequencerAPI.hpp>
#include <BackendApp.hpp>
#include <tuple>
#include <sequencer/Sequencer.hpp>

namespace node_api {

#define EXPORT_FN(x) exports.Set(#x, Napi::Function::New(env, x))

void initSequencer(Napi::Env env, Napi::Object exports) {
	EXPORT_FN(sequencerIsActive);
	EXPORT_FN(sequencerIsRecording);
	EXPORT_FN(sequencerActivate);
	EXPORT_FN(sequencerDeactivate);
	EXPORT_FN(sequencerStartRecording);
	EXPORT_FN(sequencerStopRecording);
	EXPORT_FN(sequencerRemoveSample);
	EXPORT_FN(sequencerClear);
	EXPORT_FN(sequencerLength);
	EXPORT_FN(sequencerMoveSample);
	EXPORT_FN(sequencerSwapSamples);
	EXPORT_FN(sequencerAddSample);
}

auto lockSequencer() {
	return std::make_pair(std::unique_lock(mutex), std::ref(synthesiser->getSequencer()));
}

Napi::Value sequencerIsActive(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	auto&& [_, seq] = lockSequencer();
	return Napi::Boolean::New(env, seq.isActive());
}

Napi::Value sequencerIsRecording(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	auto&& [_, seq] = lockSequencer();
	return Napi::Boolean::New(env, seq.isRecording());
}

void sequencerActivate(const Napi::CallbackInfo& info) {
	auto&& [_, seq] = lockSequencer();
	seq.activate();
}

void sequencerDeactivate(const Napi::CallbackInfo& info) {
	auto&& [_, seq] = lockSequencer();
	seq.deactivate();
}

void sequencerStartRecording(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 3) {
		Napi::Error::New(env, fmt::format("expected 3 arguments, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto sampleRateVal = info[0];
	if (not sampleRateVal.IsNumber()) {
		Napi::Error::New(env, "sampleRate is not a Number").ThrowAsJavaScriptException();
	}

	auto channelsVal = info[1];
	if (not channelsVal.IsNumber()) {
		Napi::Error::New(env, "channels is not a Number").ThrowAsJavaScriptException();
	}

	auto secondsVal = info[2];
	if (not secondsVal.IsNumber()) {
		Napi::Error::New(env, "seconds is not a Number").ThrowAsJavaScriptException();
	}

	auto sampleRate = sampleRateVal.ToNumber().Uint32Value();
	auto channels = channelsVal.ToNumber().Uint32Value();
	auto seconds = secondsVal.ToNumber().FloatValue();

	auto&& [_, seq] = lockSequencer();
	seq.startRecording(sampleRate, channels, seconds);
}

void sequencerStopRecording(const Napi::CallbackInfo& info) {
	auto&& [_, seq] = lockSequencer();
	seq.stopRecording();
}

void sequencerRemoveSample(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 1) {
		Napi::Error::New(env, fmt::format("expected 1 argument, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto iVal = info[0];
	if (not iVal.IsNumber()) {
		Napi::Error::New(env, "i is not a Number").ThrowAsJavaScriptException();
	}

	auto i = iVal.ToNumber().Uint32Value();

	auto&& [_, seq] = lockSequencer();
	seq.removeSample(i);
}

void sequencerClear(const Napi::CallbackInfo& info) {
	auto&& [_, seq] = lockSequencer();
	seq.clear();
}

Napi::Value sequencerLength(const Napi::CallbackInfo& info) {
	auto env = info.Env();
	auto&& [_, seq] = lockSequencer();
	return Napi::Number::New(env, seq.length());
}

void sequencerMoveSample(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 2) {
		Napi::Error::New(env, fmt::format("expected 2 arguments, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto currVal = info[0];
	if (not currVal.IsNumber()) {
		Napi::Error::New(env, "curr is not a Number").ThrowAsJavaScriptException();
	}

	auto targetVal = info[1];
	if (not targetVal.IsNumber()) {
		Napi::Error::New(env, "target is not a Number").ThrowAsJavaScriptException();
	}

	auto curr = currVal.ToNumber().Uint32Value();
	auto target = targetVal.ToNumber().Uint32Value();

	auto&& [_, seq] = lockSequencer();
	seq.moveSample(curr, target);
}

void sequencerSwapSamples(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 2) {
		Napi::Error::New(env, fmt::format("expected 2 arguments, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto i1Val = info[0];
	if (not i1Val.IsNumber()) {
		Napi::Error::New(env, "i1 is not a Number").ThrowAsJavaScriptException();
	}

	auto i2Val = info[0];
	if (not i2Val.IsNumber()) {
		Napi::Error::New(env, "i2 is not a Number").ThrowAsJavaScriptException();
	}

	auto i1 = i1Val.ToNumber().Uint32Value();
	auto i2 = i2Val.ToNumber().Uint32Value();

	auto&& [_, seq] = lockSequencer();
	seq.swapSamples(i1, i2);
}

void sequencerAddSample(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 1) {
		Napi::Error::New(env, fmt::format("expected 1 argument, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto nameVal = info[0];
	if (not nameVal.IsString()) {
		Napi::Error::New(env, "name is not a Number").ThrowAsJavaScriptException();
	}

	auto name = nameVal.ToString().Utf8Value();

	auto&& [_, seq] = lockSequencer();
	seq.addSampleFromFile(name);
}

}