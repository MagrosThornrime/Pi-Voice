#pragma once

#include <fileio/SampleManager.hpp>
#include "MemoryRecorder.hpp"

namespace seq {

class Sequencer;

/// @brief Iterator for sequencer
/// @brief Holds lock 
/// @brief Advances internal state of the sequencer
class SequencerIterator {
public:
	SequencerIterator() = default;
	/// @brief Creates iterator and obtains lock on sequencer
	SequencerIterator(Sequencer* sequencer, std::unique_lock<std::mutex> lock);
	/// @brief Saves sequencer state and releases lock
	~SequencerIterator();

	/// @brief Derefrerence operator
	f32 operator*() const;
	/// @brief Increment operator
	SequencerIterator& operator++();

private:
	Sequencer* _sequencer{};
	u32 _sample{};
	u32 _pos;
	const std::vector<std::vector<f32>>* _samples{};
	std::unique_lock<std::mutex> _lock;
};

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

	/// @brief Returns iterator to next sample
	/// @brief IMPORTANT! The sequencer is locked during the lifetime of the iterator
	SequencerIterator iter();

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
	/// @brief Returns sample count
	u32 length() const;
	/// @brief Returns sample length in seconds
	f32 sampleLength(const u32 i) const;

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
	friend class SequencerIterator;

	mutable std::mutex _mutex;
	std::shared_ptr<fileio::SampleManager> _sampleManager;
	MemoryRecorder _recorder;
	std::vector<std::vector<f32>> _samples;
	u32 _sample{};
	u32 _pos{};
	f32 _channels{};
	f32 _sampleRate{};
	bool _active = false;
};

}