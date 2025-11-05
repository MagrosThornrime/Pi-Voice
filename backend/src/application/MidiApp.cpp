#include <application/App.hpp>

namespace application {
void MidiApp::_midiHandler(const midi::Data data){
    auto lock = std::lock_guard(_mutex);
    if (!synthesiser.isRunning()){
        return;
    }
    if (!data.hasNote()){
        return;
    }
    i32 note = data.note().num;
    if (data.status() == midi::Data::noteOn) {
        synthesiser->pressKey(note);
    } else if (data.status() == midi::Data::noteOff) {
        synthesiser->releaseKey(note);
    }
}

std::vector<midi::Port> MidiApp::listMidiPorts(){
    return midi::Ports::locked(midi::Ports::list);
}

void MidiApp::openMidiPort(i32 index){
    _midiReader.open(midi::Ports::locked(midi::Ports::getByNum, index);
    _midiReader.setGeneralCallback(_midiHandler);
}

MidiApp::MidiApp(std::shared_ptr<Synthesiser> synthesiser)
    : synthesiser(synthesiser) {}

}