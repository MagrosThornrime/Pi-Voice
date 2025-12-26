#pragma once
#include <napi.h>

namespace node_api {

void initFiles(Napi::Env env, Napi::Object exports);
void startRecording(const Napi::CallbackInfo& info);
void stopRecording(const Napi::CallbackInfo& info);
void setRecordingPath(const Napi::CallbackInfo& info);
void setSamplesPath(const Napi::CallbackInfo& info);
Napi::Array getOscillatorNames(const Napi::CallbackInfo& info);
Napi::Array getOscillatorPlot(const Napi::CallbackInfo& info);

}