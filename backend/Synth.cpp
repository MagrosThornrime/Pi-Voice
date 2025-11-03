#include <portaudiocpp/PortAudioCpp.hxx>
#include <thread>
#include <chrono>
#include <Midi.hpp>
#include <fmt/core.h>
#include <polyphonic/VoiceManager.hpp>
#include <pipeline/Pipeline.hpp>
#include <Filters.hpp>
#include <fileio/FileRecorder.hpp>


int main() {

	try {
		portaudio::AutoSystem autoSys;

		portaudio::System& sys = portaudio::System::instance();

		portaudio::Device& outputDevice = sys.defaultOutputDevice();

		std::cout << "Using output device: " << outputDevice.name() << std::endl;

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

		auto voiceManager = std::make_shared<VoiceManager>(128, 44100.0f);
		voiceManager->setOscillatorType(oscillators::square, 0);
        voiceManager->setOscillatorType(oscillators::triangle, 1);
        voiceManager->setOscillatorType(oscillators::sawtooth, 2);
		voiceManager->setAttack(0.0004f);
		voiceManager->setDecay(0.0001f);
		voiceManager->setSustain(0.5f);
		voiceManager->setRelease(0.0001f);

		auto recorder = std::make_shared<fileio::FileRecorder>("capture_in.wav", 2, 44100);
		if (!recorder->start()) {
			std::cerr << "Failed to start recorder\n";
			return 1;
		}

		Pipeline pipeline;
		pipeline
			.setSource(voiceManager)
			.addLayer(std::make_shared<LowPassFilter>(2, streamParams.outputParameters().numChannels(), 1000, streamParams.sampleRate()))
			.addLayer(recorder)
			//.addLayer(std::make_shared<LowPassFilter2>(1000, streamParams.sampleRate(), streamParams.outputParameters().numChannels()))
			//.addLayer(std::make_shared<BandPassFilter>(1000, 1, streamParams.sampleRate(), streamParams.outputParameters().numChannels()));
			;

		portaudio::CallbackInterface& pipelineInt = pipeline;
		portaudio::InterfaceCallbackStream stream(
			streamParams,
			pipeline
		);

		stream.start();
		fmt::println("Stream started");

		{
			auto midiThread = std::jthread([&voiceManager](std::stop_token stopToken) {
				try {
					midi::Reader reader;
					reader.open(midi::Ports::getByNum(1)); // set to 0 if using Windows

					for (; not stopToken.stop_requested();) {
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
						auto data = reader.read();
						if (!data.hasNote()){
							continue;
						}
						if(data.status() == midi::Data::noteOn){
							voiceManager->turnOn(data.note().num);
						}
						else if(data.status() == midi::Data::noteOff){
							voiceManager->turnOff(data.note().num);
						}
					}
				} catch (std::exception& e) {
					fmt::println("{}", e.what());
				}
			});

			(void)getchar();
		}

		recorder->stop();
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