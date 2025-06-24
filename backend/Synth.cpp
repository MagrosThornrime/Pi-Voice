#include <portaudiocpp/PortAudioCpp.hxx>
#include <iostream>
#include <thread>
#include <chrono>
#include <oscillators/SawtoothOscillator.hpp>
#include <oscillators/SineOscillator.hpp>
#include <oscillators/SquareOscillator.hpp>
#include <oscillators/TriangleOscillator.hpp>
#include <oscillators/NoiseOscillator.hpp>

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

        TriangleOscillator oscillator(44100.0f);
        oscillator.setFrequency(1, 3);
        //oscillator.setAmplitude(0.05f);

        // Use BlockingStream instead of MemFunCallbackStream
        portaudio::MemFunCallbackStream<Oscillator> stream(
            streamParams,
            oscillator,
            &TriangleOscillator::paCallback
        );

        stream.start();
        std::cout << "Stream started." << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(10));

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
