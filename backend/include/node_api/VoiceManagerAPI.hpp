#pragma once
#include <napi.h>

namespace node_api{

void initVoiceManager(Napi::Env env, Napi::Object exports);
void setAmplitude(const Napi::CallbackInfo& info);
void setOscillatorType(const Napi::CallbackInfo& info);
void setAttack(const Napi::CallbackInfo& info);
void setDecay(const Napi::CallbackInfo& info);
void setSustain(const Napi::CallbackInfo& info);
void setRelease(const Napi::CallbackInfo& info);

}