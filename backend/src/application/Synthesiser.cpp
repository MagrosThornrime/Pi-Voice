#include <application/Synthesiser.hpp>

namespace application {

Synthesiser::Synthesiser(const std::string& recordingPath, i32 channels, i32 sampleRate,
	const std::string& samplesPath): _sampleRate(sampleRate), _channels(channels) {
	_recorder = std::make_shared<fileio::FileRecorder>((u32)sampleRate, (u32)channels);
	_autoSys = std::make_unique<portaudio::AutoSystem>();
	_sampleManager = std::make_shared<fileio::SampleManager>(samplesPath, _sampleRate);
	_voiceManager = std::make_shared<polyphonic::VoiceManager>(128, (f32)sampleRate, channels, _sampleManager);
	_pipeline = std::make_shared<pipeline::Pipeline>(256, channels, _voiceManager, _recorder);

}

void Synthesiser::start() {
	auto lock = std::lock_guard(_mutex);
	if (_running) {
		return;
	}
	auto& sys = portaudio::System::instance();
	auto& outputDevice = sys.defaultOutputDevice();

	portaudio::DirectionSpecificStreamParameters outParams(
		outputDevice,
		_channels,
		portaudio::FLOAT32,
		true,
		outputDevice.defaultLowOutputLatency(),
		nullptr
	);

	portaudio::StreamParameters streamParams(
		portaudio::DirectionSpecificStreamParameters::null(),
		outParams,
		_sampleRate,
		256,
		paClipOff
	);
	_voiceManager->setOscillatorType("square", 0);

	auto& pipelineRef = *_pipeline.get();
	_stream = std::make_unique<portaudio::InterfaceCallbackStream>(streamParams, pipelineRef);
	_stream->start();
	_running = true;
}

void Synthesiser::stop() {
	auto lock = std::lock_guard(_mutex);
	if (!_running) {
		return;
	}
	_recorder->stop();
	_stream->stop();
	_stream->close();
	_running = false;
}

void Synthesiser::pressKey(i32 key) {
	auto lock = std::lock_guard(_mutex);
	_voiceManager->turnOn(key);
}

void Synthesiser::releaseKey(i32 key) {
	auto lock = std::lock_guard(_mutex);
	_voiceManager->turnOff(key);
}

bool Synthesiser::isRunning() {
	auto lock = std::lock_guard(_mutex);
	return _running;
}

void Synthesiser::setAmplitude(f32 amplitude) {
	auto lock = std::lock_guard(_mutex);
	_voiceManager->setAmplitude(amplitude);
}

void Synthesiser::setOscillatorType(const std::string& type, i32 index) {
	auto lock = std::lock_guard(_mutex);
	_voiceManager->setOscillatorType(type, index);
}

void Synthesiser::setOscillatorAmplitude(f32 amplitude, i32 index) {
	auto lock = std::lock_guard(_mutex);
	_voiceManager->setOscillatorAmplitude(amplitude, index);
}

void Synthesiser::setAttack(f32 attack) {
	auto lock = std::lock_guard(_mutex);
	_voiceManager->setAttack(attack);
}

void Synthesiser::setDecay(f32 decay) {
	auto lock = std::lock_guard(_mutex);
	_voiceManager->setDecay(decay);
}

void Synthesiser::setSustain(f32 sustain) {
	auto lock = std::lock_guard(_mutex);
	_voiceManager->setSustain(sustain);
}

void Synthesiser::setRelease(f32 release) {
	auto lock = std::lock_guard(_mutex);
	_voiceManager->setRelease(release);
}

void Synthesiser::startRecording() {
	auto lock = std::lock_guard(_mutex);
	_recorder->start();
}

void Synthesiser::stopRecording() {
	auto lock = std::lock_guard(_mutex);
	_recorder->stop();
}

void Synthesiser::setRecordingPath(const std::string& path) {
	auto lock = std::lock_guard(_mutex);
	_recorder->setOutputDirectory(path);
}

pipeline::Pipeline& Synthesiser::getPipeline() {
	return *_pipeline;
}

void Synthesiser::setSamplesPath(const std::string& path) {
	auto lock = std::lock_guard(_mutex);
	if (_voiceManager->hasActiveVoices()) {
		throw std::logic_error("There are voices active");
	}
	_sampleManager->clearCache();
	_sampleManager->changeSamplesDirectory(path);
}

std::vector<std::string> Synthesiser::getSampleNames() {
	auto lock = std::lock_guard(_mutex);
	return _sampleManager->getSampleNames();
}

std::vector<f32> Synthesiser::getOscillatorPlot(const std::string& name, i32 length) {
	if (length <= 0) {
		throw std::invalid_argument("length must be greater than 0");
	}
	const i32 note = 69;
	std::unique_ptr<oscillators::Oscillator> oscillator;
	auto lock = std::lock_guard(_mutex);
	if (name == "empty") {
		oscillator = std::make_unique<oscillators::Oscillator>((f32)_sampleRate, note);
	} else if (name == "sine") {
		oscillator = std::make_unique<oscillators::SineOscillator>((f32)_sampleRate, note);
	} else if (name == "sawtooth") {
		oscillator = std::make_unique<oscillators::SawtoothOscillator>((f32)_sampleRate, note);
	} else if (name == "square") {
		oscillator = std::make_unique<oscillators::SquareOscillator>((f32)_sampleRate, note);
	} else if (name == "triangle") {
		oscillator = std::make_unique<oscillators::TriangleOscillator>((f32)_sampleRate, note);
	} else {
		const std::vector<f32>& sample = _sampleManager->getSample(name);
		oscillator = std::make_unique<oscillators::ModulatedOscillator>((f32)_sampleRate, note, sample);
	}
	std::vector<f32> plot;
	for (i32 i = 0; i < length; i++) {
		plot.push_back(oscillator->getNextSample());
		oscillator->advance();
	}
	return plot;
}

}