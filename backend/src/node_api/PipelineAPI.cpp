#include <node_api/PipelineAPI.hpp>
#include <pipeline/Pipeline.hpp>
#include <BackendApp.hpp>
#include <tuple>
#include <Filters.hpp>
#include <Effects.hpp>
#include <typeinfo>
#ifdef __GNUG__
#include <cxxabi.h>
#include <cstdlib>
#endif

std::string demangle(const char* typeName) {
#if defined(__GNUG__)
	int status = 0;
	char* demangled = abi::__cxa_demangle(typeName, nullptr, nullptr, &status);
	std::string result = (status == 0 && demangled != nullptr) ? demangled : typeName;
	std::free(demangled);
	return result;
#else
	return typeName;
#endif
}

namespace node_api {

void initFilters(Napi::Env env, Napi::Object exports) {
	exports.Set("pipelineAddFilter", Napi::Function::New(env, addFilter));
	exports.Set("pipelineAddEffect", Napi::Function::New(env, addEffect));
	exports.Set("pipelineRemove", Napi::Function::New(env, remove));
	exports.Set("pipelineMove", Napi::Function::New(env, move));
	exports.Set("pipelineSwap", Napi::Function::New(env, swap));
	exports.Set("pipelineGetFilterParam", Napi::Function::New(env, getFilterParam));
	exports.Set("pipelineSetFilterParam", Napi::Function::New(env, setFilterParam));
	exports.Set("pipelineGetEffectParam", Napi::Function::New(env, getEffectParam));
	exports.Set("pipelineSetEffectParam", Napi::Function::New(env, setEffectParam));
	exports.Set("pipelineLength", Napi::Function::New(env, length));
}

auto lockPipeline() {
	return std::make_pair(std::unique_lock(mutex), std::ref(synthesiser->getPipeline()));
}

void printState(pipeline::Pipeline& p) {
	fmt::println("Pipeline state:");
	for (auto i = 0; i != p.length(); ++i) {
		fmt::println("\t{}: {}", i, demangle(typeid(*p.get(i)).name()));
	}
}

void addFilter(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 2) {
		Napi::Error::New(env, fmt::format("expected 2 arguments, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto typeVal = info[0];
	if (not typeVal.IsNumber()) {
		Napi::Error::New(env, "type was not Number").ThrowAsJavaScriptException();
	}

	auto idxVal = info[1];
	if (not idxVal.IsNumber()) {
		Napi::Error::New(env, "idx was not Number").ThrowAsJavaScriptException();
	}

	auto type = typeVal.ToNumber().Uint32Value();
	auto idx = idxVal.ToNumber().Uint32Value();

	auto&& [lock, p] = lockPipeline();

	p.add(filters::BwFilter::create((filters::FilterType::Value)type), idx);

	//fmt::println("Added filter {} to index {}", type, idx);
	printState(p);
}

void addEffect(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 2) {
		Napi::Error::New(env, fmt::format("expected 2 arguments, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto typeVal = info[0];
	if (not typeVal.IsNumber()) {
		Napi::Error::New(env, "type was not Number").ThrowAsJavaScriptException();
	}

	auto idxVal = info[1];
	if (not idxVal.IsNumber()) {
		Napi::Error::New(env, "idx was not Number").ThrowAsJavaScriptException();
	}

	auto type = typeVal.ToNumber().Uint32Value();
	auto idx = idxVal.ToNumber().Uint32Value();

	auto&& [lock, p] = lockPipeline();

	p.add(effects::Effect::create((effects::EffectType::Value)type), idx);

	//fmt::println("Added filter {} to index {}", type, idx);
	printState(p);
}

void remove(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 1) {
		Napi::Error::New(env, fmt::format("expected 1 argument, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto idxVal = info[0];
	if (not idxVal.IsNumber()) {
		Napi::Error::New(env, "idx was not Number").ThrowAsJavaScriptException();
	}

	auto idx = idxVal.ToNumber().Uint32Value();

	auto&& [lock, p] = lockPipeline();
	(void)p.remove(idx);
	//fmt::println("Removed filter from index {}", idx);
	printState(p);
}

void move(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 2) {
		Napi::Error::New(env, fmt::format("expected 2 arguments, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto currVal = info[0];
	if (not currVal.IsNumber()) {
		Napi::Error::New(env, "curr was not Number").ThrowAsJavaScriptException();
	}

	auto targetVal = info[1];
	if (not targetVal.IsNumber()) {
		Napi::Error::New(env, "target was not Number").ThrowAsJavaScriptException();
	}

	auto curr = currVal.ToNumber().Uint32Value();
	auto target = targetVal.ToNumber().Uint32Value();

	auto&& [lock, p] = lockPipeline();

	p.move(curr, target);

	//fmt::println("Moved filter from {} to {}", curr, target);
	printState(p);
}

void swap(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 2) {
		Napi::Error::New(env, fmt::format("expected 2 arguments, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto i1Val = info[0];
	if (not i1Val.IsNumber()) {
		Napi::Error::New(env, "i1 was not Number").ThrowAsJavaScriptException();
	}

	auto i2Val = info[1];
	if (not i2Val.IsNumber()) {
		Napi::Error::New(env, "i2 was not Number").ThrowAsJavaScriptException();
	}

	auto i1 = i1Val.ToNumber().Uint32Value();
	auto i2 = i2Val.ToNumber().Uint32Value();

	auto&& [lock, p] = lockPipeline();

	p.swap(i1, i2);
	printState(p);
	//fmt::println("Swapped filter with {} to {}", i1, i2);
}

void setFilterParam(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 3) {
		Napi::Error::New(env, fmt::format("expected 3 arguments, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto idxVal = info[0];
	if (not idxVal.IsNumber()) {
		Napi::Error::New(env, "idx was not Number").ThrowAsJavaScriptException();
	}

	auto paramVal = info[1];
	if (not paramVal.IsNumber()) {
		Napi::Error::New(env, "param was not Number").ThrowAsJavaScriptException();
	}

	auto valueVal = info[2];
	if (not valueVal.IsNumber()) {
		Napi::Error::New(env, "value was not Number").ThrowAsJavaScriptException();
	}

	auto idx = idxVal.ToNumber().Uint32Value();
	auto param = paramVal.ToNumber().Uint32Value();
	if (param >= filters::FilterParams::_count) {
		Napi::Error::New(env, fmt::format("param id = {} is invalid", param)).ThrowAsJavaScriptException();
	}
	auto value = valueVal.ToNumber().DoubleValue();

	auto&& [lock, p] = lockPipeline();

	auto layerOpt = p.get(idx);
	if (not layerOpt) {
		Napi::Error::New(env, fmt::format("no filter on idx = {}", idx)).ThrowAsJavaScriptException();
	}
	auto&& layer = *layerOpt;

#define SET_FILTER_PARAM(name) \
	case name: \
		layer.setParam( \
			param, \
			std::any((filters::FilterParams::type<name>)value) \
		); \
		layer.refresh(); \
		break

	switch (param) {
		SET_FILTER_PARAM(filters::FilterParams::cutoff);
		SET_FILTER_PARAM(filters::FilterParams::gainDB);
		SET_FILTER_PARAM(filters::FilterParams::order);
		SET_FILTER_PARAM(filters::FilterParams::quality);
	}

	fmt::println("Changed param {} for filter {} to value {}", param, idx, value);
}

Napi::Value getFilterParam(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 2) {
		Napi::Error::New(env, fmt::format("expected 2 arguments, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto idxVal = info[0];
	if (not idxVal.IsNumber()) {
		Napi::Error::New(env, "idx was not Number").ThrowAsJavaScriptException();
	}

	auto paramVal = info[1];
	if (not paramVal.IsNumber()) {
		Napi::Error::New(env, "param was not Number").ThrowAsJavaScriptException();
	}

	auto idx = idxVal.ToNumber().Uint32Value();
	auto param = paramVal.ToNumber().Uint32Value();
	if (param >= filters::FilterParams::_count) {
		Napi::Error::New(env, fmt::format("param id = {} is invalid", param)).ThrowAsJavaScriptException();
	}

	auto&& [lock, p] = lockPipeline();

	auto layerOpt = p.get(idx);
	if (not layerOpt) {
		Napi::Error::New(env, fmt::format("no filter on idx = {}", idx)).ThrowAsJavaScriptException();
	}
	auto&& layer = *layerOpt;

#define GET_FILTER_PARAM(name) \
	case name: \
		return Napi::Value::From(env, std::any_cast<filters::FilterParams::type<name>>(layer.getParam(param))); \

	switch (param) {
		GET_FILTER_PARAM(filters::FilterParams::cutoff);
		GET_FILTER_PARAM(filters::FilterParams::gainDB);
		GET_FILTER_PARAM(filters::FilterParams::order);
		GET_FILTER_PARAM(filters::FilterParams::quality);
	}

	return Napi::Number::New(env, std::nan(""));
}

void setEffectParam(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 3) {
		Napi::Error::New(env, fmt::format("expected 3 arguments, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto idxVal = info[0];
	if (not idxVal.IsNumber()) {
		Napi::Error::New(env, "idx was not Number").ThrowAsJavaScriptException();
	}

	auto paramVal = info[1];
	if (not paramVal.IsNumber()) {
		Napi::Error::New(env, "param was not Number").ThrowAsJavaScriptException();
	}

	auto valueVal = info[2];
	if (not valueVal.IsNumber()) {
		Napi::Error::New(env, "value was not Number").ThrowAsJavaScriptException();
	}

	auto idx = idxVal.ToNumber().Uint32Value();
	auto param = paramVal.ToNumber().Uint32Value();
	if (param >= filters::FilterParams::_count) {
		Napi::Error::New(env, fmt::format("param id = {} is invalid", param)).ThrowAsJavaScriptException();
	}
	auto value = valueVal.ToNumber().DoubleValue();

	auto&& [lock, p] = lockPipeline();

	auto layerOpt = p.get(idx);
	if (not layerOpt) {
		Napi::Error::New(env, fmt::format("no effect on idx = {}", idx)).ThrowAsJavaScriptException();
	}
	auto&& layer = *layerOpt;

	auto effect = (effects::Effect*)&layer;

#define SET_EFFECT_PARAM(params, name) \
	case name: \
		layer.setParam( \
			param, \
			std::any((params::type<name>)value) \
		); \
		layer.refresh(); \
		break

	switch (effect->getEffectType()) {

	case effects::EffectType::chorus:
		switch (param) {
			SET_EFFECT_PARAM(effects::ChorusParams, effects::ChorusParams::baseDelayFactor);
			SET_EFFECT_PARAM(effects::ChorusParams, effects::ChorusParams::modFrequency);
			SET_EFFECT_PARAM(effects::ChorusParams, effects::ChorusParams::modDepth);
			SET_EFFECT_PARAM(effects::ChorusParams, effects::ChorusParams::bufferFrames);
			SET_EFFECT_PARAM(effects::ChorusParams, effects::ChorusParams::feedback);
			SET_EFFECT_PARAM(effects::ChorusParams, effects::ChorusParams::wetAmount);
		}
		break;

	case effects::EffectType::reverb:
		switch (param) {
			SET_EFFECT_PARAM(effects::ReverbParams, effects::ReverbParams::bufferSize);
			SET_EFFECT_PARAM(effects::ReverbParams, effects::ReverbParams::feedback);
			SET_EFFECT_PARAM(effects::ReverbParams, effects::ReverbParams::wetAmount);
		}
		break;

	case effects::EffectType::delay:
		switch (param) {
			SET_EFFECT_PARAM(effects::DelayParams, effects::DelayParams::delayTime);
			SET_EFFECT_PARAM(effects::DelayParams, effects::DelayParams::feedback);
			SET_EFFECT_PARAM(effects::DelayParams, effects::DelayParams::wetAmount);
		}
		break;

	case effects::EffectType::robotify:
		switch (param) {
			SET_EFFECT_PARAM(effects::RobotifyParams, effects::RobotifyParams::modFrequency);
		}
		break;
	}


	fmt::println("Changed param {} for filter {} to value {}", param, idx, value);
}

Napi::Value getEffectParam(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	if (info.Length() != 2) {
		Napi::Error::New(env, fmt::format("expected 2 arguments, got {}", info.Length())).ThrowAsJavaScriptException();
	}

	auto idxVal = info[0];
	if (not idxVal.IsNumber()) {
		Napi::Error::New(env, "idx was not Number").ThrowAsJavaScriptException();
	}

	auto paramVal = info[1];
	if (not paramVal.IsNumber()) {
		Napi::Error::New(env, "param was not Number").ThrowAsJavaScriptException();
	}

	auto idx = idxVal.ToNumber().Uint32Value();
	auto param = paramVal.ToNumber().Uint32Value();
	if (param >= filters::FilterParams::_count) {
		Napi::Error::New(env, fmt::format("param id = {} is invalid", param)).ThrowAsJavaScriptException();
	}

	auto&& [lock, p] = lockPipeline();

	auto layerOpt = p.get(idx);
	if (not layerOpt) {
		Napi::Error::New(env, fmt::format("no effect on idx = {}", idx)).ThrowAsJavaScriptException();
	}
	auto&& layer = *layerOpt;
	auto effect = (effects::Effect*)&layer;

#define GET_FILTER_PARAM(params, name) \
	case name: \
		return Napi::Value::From(env, std::any_cast<params::type<name>>(layer.getParam(param))); \

	switch (effect->getEffectType()) {

	case effects::EffectType::chorus:
		switch (param) {
			GET_FILTER_PARAM(effects::ChorusParams, effects::ChorusParams::baseDelayFactor);
			GET_FILTER_PARAM(effects::ChorusParams, effects::ChorusParams::modFrequency);
			GET_FILTER_PARAM(effects::ChorusParams, effects::ChorusParams::modDepth);
			GET_FILTER_PARAM(effects::ChorusParams, effects::ChorusParams::bufferFrames);
			GET_FILTER_PARAM(effects::ChorusParams, effects::ChorusParams::feedback);
			GET_FILTER_PARAM(effects::ChorusParams, effects::ChorusParams::wetAmount);
		}
		break;

	case effects::EffectType::reverb:
		switch (param) {
			GET_FILTER_PARAM(effects::ReverbParams, effects::ReverbParams::bufferSize);
			GET_FILTER_PARAM(effects::ReverbParams, effects::ReverbParams::feedback);
			GET_FILTER_PARAM(effects::ReverbParams, effects::ReverbParams::wetAmount);
		}
		break;

	case effects::EffectType::delay:
		switch (param) {
			GET_FILTER_PARAM(effects::DelayParams, effects::DelayParams::delayTime);
			GET_FILTER_PARAM(effects::DelayParams, effects::DelayParams::feedback);
			GET_FILTER_PARAM(effects::DelayParams, effects::DelayParams::wetAmount);
		}
		break;

	case effects::EffectType::robotify:
		switch (param) {
			GET_FILTER_PARAM(effects::RobotifyParams, effects::RobotifyParams::modFrequency);
		}
		break;
	}

	return Napi::Number::New(env, std::nan(""));
}

Napi::Value length(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	auto&& [lock, p] = lockPipeline();

	return Napi::Value::From(env, p.length());
}

}