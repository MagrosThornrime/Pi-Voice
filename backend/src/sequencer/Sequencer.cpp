#include <sequencer/Sequencer.hpp>

namespace seq {

Sequencer::Sequencer(std::shared_ptr<fileio::SampleManager> sampleManager):
	_sampleManager{std::move(sampleManager)} {}

bool Sequencer::isActive() const {
	return _active;
}

bool Sequencer::isRecording() const {
	return _recorder.running();
}

void Sequencer::activate() {
	if (not _recorder.running()) {
		_active = true;
	}
}

void Sequencer::deactivate() {
	if (not _recorder.running()) {
		_active = false;
	}
}

void Sequencer::startRecording(const u32 sampleRate, const u32 channels, const f32 seconds) {
	if (not _active and not _recorder.running()) {
		_recorder.set(sampleRate, channels, seconds);
		_recorder.start();
	}
}

void Sequencer::writeToRecorder(std::span<const f32> data) {
	if (not _active and _recorder.running()) {
		_recorder.write(data);
	}
}


void Sequencer::stopRecording() {
	if (not _active and _recorder.running()) {
		_recorder.stop();
		_samples.push_back(_recorder.getResult());
	}
}

void Sequencer::removeSample(const u32 i) {
	if (not _active and not _recorder.running() and i < _samples.size()) {
		_samples.erase(_samples.begin() + i);
	}
}

void Sequencer::clear() {
	if (not _active and not _recorder.running()) {
		_samples.clear();
	}
}

void Sequencer::addSample(std::vector<f32> sample) {
	if (not _active and not _recorder.running()) {
		_samples.push_back(sample);
	}
}

void Sequencer::addSampleFromFile(const std::string& name) {
	if (not _active and not _recorder.running()) {
		_samples.push_back(_sampleManager->getSample(name));
	}
}

void Sequencer::moveSample(const u32 curr, const u32 target) {
	if (not _active and not _recorder.running()) {
		const u32 size = _samples.size();
		if (curr >= size) {
			return;
		}

		auto&& item = std::move(_samples[curr]);
		_samples.erase(_samples.begin() + curr);

		u32 insertIndex = std::min(target, size);
		_samples.insert(_samples.begin() + insertIndex, std::move(item));
	}
}

void Sequencer::swapSamples(const u32 i1, const u32 i2) {
	if (i1 >= _samples.size() || i2 >= _samples.size()) {
		return;
	}
	std::swap(_samples[i1], _samples[i2]);
}

}