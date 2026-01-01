#include <application/MidiManager.hpp>
#include <application/Synthesiser.hpp>
#include <napi.h>
#include <Types.hpp>
#include <BackendApp.hpp>
#include <NodeAPI.hpp>

std::shared_ptr<application::Synthesiser> synthesiser{};
std::shared_ptr<application::MidiManager> midiApp{};
std::mutex mutex{};

void initializeApplication(){
    synthesiser = std::make_shared<application::Synthesiser>("capture_in.wav", 2, 44100, "res/samples");
    midiApp = std::make_shared<application::MidiManager>(synthesiser);
    synthesiser->start();
}

void destroyApplication(void*) {
	synthesiser->stop();
	midiApp.reset();
	synthesiser.reset();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
	initializeApplication();

	node_api::initMidi(env, exports);
	node_api::initFiles(env, exports);
	node_api::initVoiceManager(env, exports);
	node_api::initFilters(env, exports);
	node_api::initSequencer(env, exports);

    env.AddCleanupHook(destroyApplication, (void*)nullptr);

	fmt::println("SynthModule initialized successfully");
	return exports;
}

NODE_API_MODULE(SynthModule, Init);