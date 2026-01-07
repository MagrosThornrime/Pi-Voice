#pragma once
#include <polyphonic/VoiceManager.hpp>
#include <portaudiocpp/PortAudioCpp.hxx>
#include <fileio/FileRecorder.hpp>
#include <thread>
#include <Types.hpp>
#include <string>
#include <pipeline/Pipeline.hpp>

namespace application {

/// @brief Controls audio playback and allows for customizing the played sound
class Synthesiser {

	std::shared_ptr<pipeline::Pipeline> _pipeline;
	std::shared_ptr<fileio::FileRecorder> _recorder;
	std::unique_ptr<portaudio::InterfaceCallbackStream> _stream;
    std::shared_ptr<polyphonic::VoiceManager> _voiceManager;
	std::shared_ptr<fileio::SampleManager> _sampleManager;
	std::shared_ptr<seq::Sequencer> _sequencer;

	/// @brief Port audio configuration data
	std::unique_ptr<portaudio::AutoSystem> _autoSys;

	std::mutex _mutex;
	i32 _sampleRate;
	i32 _channels;

	/// @brief Tells if the synthesiser is playing the sound
	bool _running = false;

public:
	/// @brief Getter of the _running variable
	bool isRunning();

    /// @brief Constructor
    /// @param recordingPath name of the audio file where you want to record the output
    /// @param channels number of audio output channels
    /// @param sampleRate sound's sample rate
	/// @param samplesPath path to a directory with samples for oscillators
    Synthesiser(const std::string& recordingPath, i32 channels, i32 sampleRate, const std::string& samplesPath);

	/// @brief Tells the voice manager that you pressed a key on the keyboard
	/// @param key id of the key
	void pressKey(i32 key);

	/// @brief Tells the voice manager that you released a key on the keyboard
	/// @param key id of the key
	void releaseKey(i32 key);

	/// @brief Starts playing the sound
	void start();

	/// @brief Stops playing the sound
	void stop();

    /// @brief Sets the type of a particular oscillator
    /// @param type type of the oscillator
    /// @param index id of the oscillator (0, 1 or 2)
    void setOscillatorType(const std::string& type, i32 index);

	/// @brief Sets the attack factor in ADSR
	/// @param attack attack factor value (between 0 and 1)
	void setAttack(f32 attack);

	/// @brief Sets the decay factor in ADSR
	/// @param decay decay factor value (between 0 and 1)
	void setDecay(f32 decay);

	/// @brief Sets the sustain amplitude in ADSR
	/// @param sustain sustain amplitude value (between 0 and 1)
	void setSustain(f32 sustain);

	/// @brief Sets the release factor value in ADSR
	/// @param release release factor value (between 0 and 1)
	void setRelease(f32 release);

	/// @brief Starts recording the audio.
	void startRecording();

	/// @brief Stops recording the audio.
	void stopRecording();

	/// @brief Tells where to put audio recordings
	/// @param path output directory
	void setRecordingPath(const std::string& path);

	/// @brief Tells where to get samples from
	/// @param path input directory
	void setSamplesPath(const std::string& path);

	/// @brief Gets all oscillators' names
	std::vector<std::string> getSampleNames();

	/// @brief Gets the oscillator output signal
	/// @param name oscillator's name
	std::vector<f32> getOscillatorPlot(const std::string& name);

	/// @brief Gets a reference to the pipeline (used by external API-s)
	pipeline::Pipeline& getPipeline();

	/// @brief Gets a reference to the sequencer (used by external API-s)
	seq::Sequencer& getSequencer();
};

}