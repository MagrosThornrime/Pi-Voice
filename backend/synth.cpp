#include <portaudiocpp/PortAudioCpp.hxx>
#include <iostream>
#include <thread>
#include <chrono>
#include <oscillators/SawtoothOscillator.hpp>

int main() {
    try {
        portaudio::AutoSystem autoSys;

        portaudio::System &sys = portaudio::System::instance();

        portaudio::Device &outputDevice = sys.defaultOutputDevice();

        std::cout << "Using output device: " << outputDevice.name() << std::endl;

        portaudio::DirectionSpecificStreamParameters outParams(
            outputDevice,
            2,                      // 2 channels (stereo)
            portaudio::FLOAT32,     // sample format
            false,                  // interleaved
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

        SawtoothOscillator oscillator(-5.0f, 5.0f, 0.01f);

        // Use BlockingStream instead of MemFunCallbackStream
        portaudio::MemFunCallbackStream<SawtoothOscillator> stream(
            streamParams,
            oscillator,
            &SawtoothOscillator::paCallback
        );

        stream.start();
        std::cout << "Stream started." << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(2));

        stream.stop();
        stream.close();
        std::cout << "Stream stopped and closed." << std::endl;

    } catch (const portaudio::PaException &e) {
        std::cerr << "PortAudio exception: " << e.paErrorText() << std::endl;
        return 1;
    } catch (const std::exception &e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
