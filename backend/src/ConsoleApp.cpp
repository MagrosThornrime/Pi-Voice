#include <application/MidiManager.hpp>
#include <application/Synthesiser.hpp>
#include <Types.hpp>
#include <range/v3/all.hpp>
#include <iostream>
#include <Effects.hpp>

int main(){
    auto synthesiser = std::make_shared<application::Synthesiser>("capture_in.wav", 2, 44100, "res/samples");
    auto midiApp = std::make_shared<application::MidiManager>(synthesiser);
    auto& pipeline = synthesiser->getPipeline();

	pipeline.add(
		std::make_shared<effects::ReverbEffect>(2, 1000, 0.1, 0.5),
		std::nullopt
	);

    synthesiser->start();
    midiApp->openMidiPort(1);
    synthesiser->setOscillatorType("square", 0);

    std::string command;
    while(std::cin >> command){
        if(command == "quit"){
            break;
        }
    }

    synthesiser->stop();
    midiApp.reset();
    synthesiser.reset();
}