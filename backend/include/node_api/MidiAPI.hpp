#pragma once
#include <napi.h>

namespace node_api{

void initMidi(Napi::Env env, Napi::Object exports);
Napi::Array getMidiPorts(const Napi::CallbackInfo& info);
void openMidiPort(const Napi::CallbackInfo& info);

}