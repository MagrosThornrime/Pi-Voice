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

	auto chorus = std::make_shared<effects::ChorusEffect>(channels, sampleRate);

	pipeline.add(
    	chorus,
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
    	else if (command == "chorus") {
        	u32 newSize;
        	std::cin >> newSize;

        	chorus->setParam(
            	effects::ChorusParams::bufferFrames,
            	newSize
        	);

        	std::cout << "Chorus buffer size set to "
            	      << newSize << std::endl;
    	}
	}

    synthesiser->stop();
    midiApp.reset();
    synthesiser.reset();
}