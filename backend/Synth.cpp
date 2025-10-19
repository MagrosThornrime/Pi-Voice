#include <portaudiocpp/PortAudioCpp.hxx>
#include <iostream>
#include <thread>
#include <chrono>
#include <oscillators/SawtoothOscillator.hpp>
#include <oscillators/SineOscillator.hpp>
#include <oscillators/SquareOscillator.hpp>
#include <oscillators/TriangleOscillator.hpp>
#include <oscillators/NoiseOscillator.hpp>
#include <Midi.hpp>
#include <fmt/core.h>
#include <pipeline/Pipeline.hpp>

class LowPassFilter: public portaudio::CallbackInterface {
public:
	LowPassFilter(float cutoffFreq, float sampleRate, int channels) {
		const float RC = 1.0f / (2.0f * 3.14159265f * cutoffFreq);
		const float dt = 1.0f / sampleRate;
		_alpha = dt / (RC + dt);

		_channels = channels;

		_prevOutputs.resize(channels, 0);
	}

	int paCallbackFun(const void* input, void* output,
		unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags) override {

		const float* in = (const float*)input;
		float* out = (float*)output;

		u32 idx = 0;
		for (u32 i = 0; i != frameCount; ++i) {
			for (u32 ch = 0; ch != _channels; ++ch, ++idx) {
				float y = _prevOutputs[ch] + _alpha * (in[idx] - _prevOutputs[ch]);
				_prevOutputs[ch] = y;
				out[idx] = y;
			}
		}

		return paContinue;
	}

private:
	std::vector<float> _prevOutputs;
	float _alpha{};
	int _channels{};
};

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

		auto oscillator = std::make_shared<SineOscillator>(44100.0f);
		oscillator->setFrequency(0);

		Pipeline pipeline;
		pipeline
			.setSource(oscillator)
			.addLayer(std::make_shared<LowPassFilter>(
				1000,
				streamParams.sampleRate(),
				streamParams.outputParameters().numChannels()
			));

		//oscillator.setAmplitude(0.05f);

		portaudio::CallbackInterface& pipelineInt = pipeline;
		portaudio::InterfaceCallbackStream stream(
			streamParams,
			pipeline
		);

		stream.start();
		std::cout << "Stream started." << std::endl;

		{
			//oscillator->setFrequency(4000);
			auto sineThread = std::jthread([&](std::stop_token stopToken) {
				try {
					for (u32 i = 0; not stopToken.stop_requested(); ++i, std::this_thread::sleep_for(std::chrono::milliseconds(10))) {
						oscillator->setFrequency(200.0f + 2800.0f * (0.5f + 0.5f * std::sin(3.14159265f * i / 100.f)));
					}
				} catch (std::exception& e) {
					fmt::println("{}", e.what());
				}
			});

			(void)getchar();
		}

		/*{
			for (auto&& port : midi::Ports::list()) {
				fmt::println("{}: {}", port.num, port.name);
			}
			fmt::print("> ");
			u32 which{};
			std::cin >> which;
			std::cin.ignore();

			auto midiThread = std::jthread([&](std::stop_token stopToken) {
				try {
					midi::Reader reader;
					reader.open(midi::Ports::getByNum(which));
					reader.readAll();

					for (; not stopToken.stop_requested();) {
						auto data = reader.read();
						if (data.hasVelocity()) {
							auto note = data.note();
							fmt::print("{}{}", note.name, data.status() == midi::Data::noteOn ? 'v' : '^');
							if (data.status() == midi::Data::noteOn) {
								oscillator.setAmplitude(1);
								oscillator.setFrequency(note.freq);
								fmt::println(" {}", data.velocity());
							}
							if (data.status() == midi::Data::noteOff) {
								oscillator.setAmplitude(0);
								fmt::println("");
							}
						}
					}
				} catch (std::exception& e) {
					fmt::println("{}", e.what());
				}
			});

			(void)getchar();
		}*/

		stream.stop();
		stream.close();
		std::cout << "Stream stopped and closed." << std::endl;

	} catch (const portaudio::PaException& e) {
		std::cerr << "PortAudio exception: " << e.paErrorText() << std::endl;
		return 1;
	} catch (const std::exception& e) {
		std::cerr << "Standard exception: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
