#include <application/Synthesiser.hpp>

namespace application {
Synthesiser::Synthesiser(const std::string& recordingPath, i32 channels, i32 sampleRate)
    : _sampleRate(sampleRate), _channels(channels) {
	_pipeline = std::make_shared<pipeline::Pipeline>();
	_recorder = std::make_shared<fileio::FileRecorder>(recordingPath, channels, sampleRate);
    _autoSys = std::make_unique<portaudio::AutoSystem>();
    _voiceManager = std::make_shared<polyphonic::VoiceManager>(128, 44100.0f);
    _pitch = std::make_shared<effects::Pitch>(1, 2.0);
}

void Synthesiser::start(){
    auto lock = std::lock_guard(_mutex);
    if(_running){
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
    _voiceManager->setOscillatorType(oscillators::square, 0);
    _recorder->start();

    auto& pipelineRef = *_pipeline.get();
    pipelineRef.setSource(_voiceManager).addLayer(_pitch).addLayer(_recorder);
    _stream = std::make_unique<portaudio::InterfaceCallbackStream>(streamParams, pipelineRef);
    _stream->start();
    _running = true;
}

void Synthesiser::stop() {
    auto lock = std::lock_guard(_mutex);
    if(!_running){
        return;
    }
    _recorder->stop();
    _stream->stop();
    _stream->close();
    _running = false;
}

void Synthesiser::pressKey(i32 key){
    auto lock = std::lock_guard(_mutex);
    _voiceManager->turnOn(key);
}

void Synthesiser::releaseKey(i32 key){
    auto lock = std::lock_guard(_mutex);
    _voiceManager->turnOff(key);
}

bool Synthesiser::isRunning(){
    auto lock = std::lock_guard(_mutex);
    return _running;
}

void Synthesiser::setAmplitude(f32 amplitude){
    auto lock = std::lock_guard(_mutex);
    _voiceManager->setAmplitude(amplitude);
}

void Synthesiser::setOscillatorType(oscillators::OscillatorType type, i32 index){
    auto lock = std::lock_guard(_mutex);
    _voiceManager->setOscillatorType(type, index);
}

void Synthesiser::setOscillatorAmplitude(f32 amplitude, i32 index){
    auto lock = std::lock_guard(_mutex);
    _voiceManager->setOscillatorAmplitude(amplitude, index);
}

void Synthesiser::setAttack(f32 attack){
    auto lock = std::lock_guard(_mutex);
    _voiceManager->setAttack(attack);
}

void Synthesiser::setDecay(f32 decay){
    auto lock = std::lock_guard(_mutex);
    _voiceManager->setDecay(decay);
}

void Synthesiser::setSustain(f32 sustain){
    auto lock = std::lock_guard(_mutex);
    _voiceManager->setSustain(sustain);
}

void Synthesiser::setRelease(f32 release){
    auto lock = std::lock_guard(_mutex);
    _voiceManager->setRelease(release);
}

}