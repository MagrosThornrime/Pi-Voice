#include <application/MidiManager.hpp>
#include <application/Synthesiser.hpp>
#include <napi.h>
#include <Types.hpp>
#include <application/BackendApp.hpp>
#include <application/PipelineAPI.hpp>
#include <range/v3/all.hpp>
#include <iostream>

int main(){
    auto synthesiser = std::make_shared<application::Synthesiser>("capture_in.wav", 2, 44100, "res/samples");
    auto midiApp = std::make_shared<application::MidiManager>(synthesiser);
    synthesiser->start();

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