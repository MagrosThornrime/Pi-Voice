#pragma once

#include <napi.h>

namespace node_api {

void initSequencer(Napi::Env env, Napi::Object exports);
Napi::Value sequencerIsActive(const Napi::CallbackInfo& info);
Napi::Value sequencerIsRecording(const Napi::CallbackInfo& info);
void sequencerActivate(const Napi::CallbackInfo& info);
void sequencerDeactivate(const Napi::CallbackInfo& info);
void sequencerStartRecording(const Napi::CallbackInfo& info);
void sequencerStopRecording(const Napi::CallbackInfo& info);
void sequencerRemoveSample(const Napi::CallbackInfo& info);
void sequencerClear(const Napi::CallbackInfo& info);
Napi::Value sequencerLength(const Napi::CallbackInfo& info);
void sequencerMoveSample(const Napi::CallbackInfo& info);
void sequencerSwapSamples(const Napi::CallbackInfo& info);
void sequencerAddSample(const Napi::CallbackInfo& info);

}