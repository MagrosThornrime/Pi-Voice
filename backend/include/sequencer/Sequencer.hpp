#pragma once

#include <fileio/SampleManager.hpp>
#include "MemoryRecorder.hpp"

namespace seq {

class Sequencer {
public:
	Sequencer(std::shared_ptr<fileio::SampleManager> sampleManager);

	bool isActive() const;
	bool isRecording() const;
	/// @brief Activates sequencer
	/// @brief Available to non-recording sequencer
	void activate();
	/// @brief Deactivates sequencer (may record)
	void deactivate();

	/// @brief Starts recording
	/// @brief Available to deactivated recorder
	void startRecording(const u32 sampleRate, const u32 channels = 2, const f32 seconds = 1);
	/// @brief Sends data to recorder
	void writeToRecorder(std::span<const f32> data);
	/// @brief Stops recording
	void stopRecording();

	/// @brief Removes i-th sample
	/// @brief Available for deactivated & non-recording sequencer
	void removeSample(const u32 i);
	/// @brief Clears all samples
	/// @brief Available for deactivated & non-recording sequencer
	void clear();

	/// @brief Adds raw sample
	/// @brief Available for deactivated & non-recording sequencer
	void addSample(std::vector<f32> sample);
	/// @brief Reads sample from file
	/// @brief Available for deactivated & non-recording sequencer
	void addSampleFromFile(const std::string& name);

	/// @brief Moves sample from curr pos to target pos
	/// @brief Available for deactivated & non-recording sequencer
	void moveSample(const u32 curr, const u32 target);
	/// @brief Swaps two samples
	/// @brief Available for deactivated & non-recording sequencer
	void swapSamples(const u32 i1, const u32 i2);

private:
	std::shared_ptr<fileio::SampleManager> _sampleManager;
	MemoryRecorder _recorder;
	std::vector<std::vector<f32>> _samples;
	bool _active = false;
};

}