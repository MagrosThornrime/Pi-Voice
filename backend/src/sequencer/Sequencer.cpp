#include <sequencer/Sequencer.hpp>

namespace seq {

Sequencer::Sequencer(std::shared_ptr<fileio::SampleManager> sampleManager):
	_sampleManager{std::move(sampleManager)} {}

bool Sequencer::isActive() const {
	auto lock = std::unique_lock(_mutex);
	return _active;
}

bool Sequencer::isRecording() const {
	auto lock = std::unique_lock(_mutex);
	return _recorder.running();
}

u32 Sequencer::length() const {
	auto lock = std::unique_lock(_mutex);
	return _samples.size();
}

void Sequencer::activate() {
	auto lock = std::unique_lock(_mutex);
	if (not _recorder.running() and not _active) {
		_active = true;
		_sample = 0;
		_pos = 0;
	}
}

void Sequencer::deactivate() {
	auto lock = std::unique_lock(_mutex);
	if (not _recorder.running()) {
		_active = false;
	}
}

SequencerIterator Sequencer::iter() {
	auto lock = std::unique_lock(_mutex);
	if (_active) {
		return SequencerIterator(this, std::move(lock));
	}
	return SequencerIterator();
}

void Sequencer::startRecording(const u32 sampleRate, const u32 channels, const f32 seconds) {
	auto lock = std::unique_lock(_mutex);
	if (not _active and not _recorder.running()) {
		_recorder.set(sampleRate, channels, seconds);
		_recorder.start();
	}
}

void Sequencer::writeToRecorder(std::span<const f32> data) {
	auto lock = std::unique_lock(_mutex);
	if (not _active and _recorder.running()) {
		if (not _recorder.write(data)) {
			_recorder.stop();
			_samples.push_back(_recorder.getResult());
		}
	}
}


void Sequencer::stopRecording() {
	auto lock = std::unique_lock(_mutex);
	if (not _active and _recorder.running()) {
		_recorder.stop();
		_samples.push_back(_recorder.getResult());
	}
}

void Sequencer::removeSample(const u32 i) {
	auto lock = std::unique_lock(_mutex);
	if (not _active and not _recorder.running() and i < _samples.size()) {
		_samples.erase(_samples.begin() + i);
	}
}

void Sequencer::clear() {
	auto lock = std::unique_lock(_mutex);
	if (not _active and not _recorder.running()) {
		_samples.clear();
	}
}

void Sequencer::addSample(std::vector<f32> sample) {
	auto lock = std::unique_lock(_mutex);
	if (not _active and not _recorder.running()) {
		_samples.push_back(sample);
	}
}

void Sequencer::addSampleFromFile(const std::string& name) {
	auto lock = std::unique_lock(_mutex);
	if (not _active and not _recorder.running()) {
		_samples.push_back(_sampleManager->getSample(name));
	}
}

void Sequencer::moveSample(const u32 curr, const u32 target) {
	auto lock = std::unique_lock(_mutex);
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
	auto lock = std::unique_lock(_mutex);
	if (i1 >= _samples.size() || i2 >= _samples.size()) {
		return;
	}
	std::swap(_samples[i1], _samples[i2]);
}

SequencerIterator::SequencerIterator(Sequencer* sequencer, std::unique_lock<std::mutex> lock):
	_sequencer{sequencer},
	_sample{_sequencer->_sample},
	_pos{_sequencer->_pos},
	_samples{&_sequencer->_samples},
	_lock{std::move(lock)} {}

SequencerIterator::~SequencerIterator() {
	_sequencer->_sample = _sample;
	_sequencer->_pos = _pos;
}

f32 SequencerIterator::operator*() const {
	return (*_samples)[_sample][_pos];
}

SequencerIterator& SequencerIterator::operator++() {
	if (++_pos >= (*_samples)[_sample].size()) {
		_pos = 0;
		if (++_sample >= _samples->size()) {
			_sample = 0;
		}
	}
	return *this;
}

}