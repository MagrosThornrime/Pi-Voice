#include <application/MidiManager.hpp>

namespace application {
void MidiManager::_midiHandler(const midi::Data data) {
	auto lock = std::lock_guard(_mutex);
	if (!_synthesiser->isRunning()) {
		return;
	}
	if (!data.hasNote()) {
		return;
	}
	i32 note = data.note().num;
	if (data.status() == midi::Data::noteOn) {
		_synthesiser->pressKey(note);
	} else if (data.status() == midi::Data::noteOff) {
		_synthesiser->releaseKey(note);
	}
}

std::vector<midi::Port> MidiManager::listMidiPorts() {
	return midi::Ports::list();
}

void MidiManager::openMidiPort(i32 index) {
	auto port = midi::Ports::getByNum(index);
	_midiReader.open(port);
	_midiReader.setGeneralCallback(
		[this](midi::Data data) { _midiHandler(data); }
	);
}

MidiManager::MidiManager(std::shared_ptr<Synthesiser> _synthesiser)
	: _synthesiser(_synthesiser) {}

}