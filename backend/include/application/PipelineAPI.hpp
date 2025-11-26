#pragma once

#include <napi.h>

namespace pipelineAPI {

void init(Napi::Env env, Napi::Object exports);

Napi::Value addFilter(const Napi::CallbackInfo& info);
Napi::Value remove(const Napi::CallbackInfo& info);
Napi::Value move(const Napi::CallbackInfo& info);
Napi::Value swap(const Napi::CallbackInfo& info);
Napi::Value setFilterParam(const Napi::CallbackInfo& info);
Napi::Value getFilterParam(const Napi::CallbackInfo& info);
Napi::Value length(const Napi::CallbackInfo& info);

}