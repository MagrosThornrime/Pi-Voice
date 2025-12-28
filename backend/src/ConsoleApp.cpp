#include <application/MidiManager.hpp>
#include <application/Synthesiser.hpp>
#include <Types.hpp>
#include <range/v3/all.hpp>
#include <iostream>

int main(){
    auto synthesiser = std::make_shared<application::Synthesiser>("capture_in.wav", 2, 44100, "res/samples");
    auto midiApp = std::make_shared<application::MidiManager>(synthesiser);
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