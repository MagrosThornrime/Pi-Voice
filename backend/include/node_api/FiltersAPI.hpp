#pragma once

#include <napi.h>

namespace node_api {

void initFilters(Napi::Env env, Napi::Object exports);
void addFilter(const Napi::CallbackInfo& info);
void remove(const Napi::CallbackInfo& info);
void move(const Napi::CallbackInfo& info);
void swap(const Napi::CallbackInfo& info);
void setFilterParam(const Napi::CallbackInfo& info);
Napi::Value getFilterParam(const Napi::CallbackInfo& info);
Napi::Value length(const Napi::CallbackInfo& info);

}