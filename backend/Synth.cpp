#include <portaudiocpp/PortAudioCpp.hxx>
#include <thread>
#include <chrono>
#include <Midi.hpp>
#include <fmt/core.h>
#include <VoiceManager.hpp>


int main() {
	try {
		portaudio::AutoSystem autoSys;

		portaudio::System& sys = portaudio::System::instance();

		portaudio::Device& outputDevice = sys.defaultOutputDevice();

		fmt::println("Using output device: {}", outputDevice.name());

		portaudio::DirectionSpecificStreamParameters outParams(
			outputDevice,
			2,                      // 2 channels (stereo)
			portaudio::FLOAT32,     // sample format
			true,                  // interleaved
			outputDevice.defaultLowOutputLatency(),
			nullptr
		);

		portaudio::StreamParameters streamParams(
			portaudio::DirectionSpecificStreamParameters::null(), // input
			outParams,
			44100,
			256,
			paClipOff
		);

		VoiceManager voiceManager(128, 44100.0f);

		// Use BlockingStream instead of MemFunCallbackStream
		portaudio::MemFunCallbackStream<VoiceManager> stream(
			streamParams,
			voiceManager,
			&VoiceManager::paCallback
		);

		stream.start();
		fmt::println("Stream started");

		{
			auto midiThread = std::jthread([&voiceManager](std::stop_token stopToken) {
				try {
					midi::Reader reader;
					reader.open(midi::Ports::getByNum(1));

					for (; not stopToken.stop_requested();) {
						auto data = reader.read();
						if (!data.hasNote()){
							continue;
						}
						if(data.status() == midi::Data::noteOn){
							voiceManager.turnOn(data.note().num);
						}
						else if(data.status() == midi::Data::noteOff){
							voiceManager.turnOff(data.note().num);
						}
					}
				} catch (std::exception& e) {
					fmt::println("{}", e.what());
				}
			});

			(void)getchar();
		}

		stream.stop();
		stream.close();
		fmt::println("Stream stopped and closed");

	} catch (const portaudio::PaException& e) {
		fmt::println("{}", e.paErrorText());
		return 1;
	} catch (const std::exception& e) {
		fmt::println("{}", e.what());
		return 1;
	}

	return 0;
}
