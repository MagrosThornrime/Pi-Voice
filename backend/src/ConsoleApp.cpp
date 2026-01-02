#include <application/MidiManager.hpp>
#include <application/Synthesiser.hpp>
#include <Types.hpp>
#include <range/v3/all.hpp>
#include <iostream>
#include <Effects.hpp>

const u32 channels = 2;
const f32 sampleRate = 44100;

int main(){
    auto synthesiser = std::make_shared<application::Synthesiser>("capture_in.wav", channels, sampleRate, "res/samples");
    auto midiApp = std::make_shared<application::MidiManager>(synthesiser);
    auto& pipeline = synthesiser->getPipeline();

	auto delay = std::make_shared<effects::DelayEffect>(channels, sampleRate);

	pipeline.add(
    	delay,
    	std::nullopt
	);


    synthesiser->start();
    midiApp->openMidiPort(1);
    synthesiser->setOscillatorType("square", 0);

	std::string command;
	while (std::cin >> command) {
    	if (command == "quit") {
        	break;
    	}
    	else if (command == "delay") {
        	u32 newSize;
        	std::cin >> newSize;

        	delay->setParam(
            	effects::DelayParams::bufferFrames,
            	newSize
        	);

        	std::cout << "Delay buffer size set to "
            	      << newSize << std::endl;
    	}
	}

    synthesiser->stop();
    midiApp.reset();
    synthesiser.reset();
}