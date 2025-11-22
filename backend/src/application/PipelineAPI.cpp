#include <application/PipelineAPI.hpp>
#include <pipeline/Pipeline.hpp>
#include <application/BackendApp.hpp>
#include <tuple>
#include <filters/BwFilter.hpp>
#include <filters/FilterParams.hpp>

namespace pipelineAPI {

void init(Napi::Env env, Napi::Object exports) {
	exports.Set("pipelineAddFilter", Napi::Function::New(env, addFilter));
	exports.Set("pipelineRemove", Napi::Function::New(env, remove));
	exports.Set("pipelineMove", Napi::Function::New(env, move));
	exports.Set("pipelineSwap", Napi::Function::New(env, swap));
	exports.Set("pipelineGetFilterParam", Napi::Function::New(env, getFilterParam));
	exports.Set("pipelineSetFilterParam", Napi::Function::New(env, setFilterParam));
	exports.Set("pipelineLength", Napi::Function::New(env, length));
}

std::tuple<std::lock_guard<std::mutex>, pipeline::Pipeline&> lockPipeline() {
	return std::tuple_cat(std::make_tuple(std::lock_guard(mutex)), std::tie(synthesiser->getPipeline()));
}

Napi::Value addFilter(const Napi::CallbackInfo& info) {
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

	p.add(filters::BwFilter::create((filters::FilterTypes::Value)type), idx);

	return env.Null();
}

Napi::Value remove(const Napi::CallbackInfo& info) {
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

	return env.Null();
}

Napi::Value move(const Napi::CallbackInfo& info) {
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

	return env.Null();
}

Napi::Value swap(const Napi::CallbackInfo& info) {
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

	return env.Null();
}

Napi::Value setFilterParam(const Napi::CallbackInfo& info) {
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
		SET_FILTER_PARAM(filters::FilterParams::channels);
		SET_FILTER_PARAM(filters::FilterParams::cutoff);
		SET_FILTER_PARAM(filters::FilterParams::gainDB);
		SET_FILTER_PARAM(filters::FilterParams::order);
		SET_FILTER_PARAM(filters::FilterParams::quality);
		SET_FILTER_PARAM(filters::FilterParams::sampleRate);
	}

	return env.Null();
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
		break

	switch (param) {
		GET_FILTER_PARAM(filters::FilterParams::channels);
		GET_FILTER_PARAM(filters::FilterParams::cutoff);
		GET_FILTER_PARAM(filters::FilterParams::gainDB);
		GET_FILTER_PARAM(filters::FilterParams::order);
		GET_FILTER_PARAM(filters::FilterParams::quality);
		GET_FILTER_PARAM(filters::FilterParams::sampleRate);
	}

	return env.Null();
}

Napi::Value length(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	auto&& [lock, p] = lockPipeline();

	return Napi::Value::From(env, p.length());
}

}