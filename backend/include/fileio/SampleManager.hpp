#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <Types.hpp>
#include <sndfile.h>
#include <mutex>
#include <string_view>


namespace fileio {
/// @brief Loads audio samples so they can be used later as oscillators
class SampleManager {
    /// @brief A directory where the samples are stored
    std::string _samplesDirectory;

	/// @brief Sound's sampling rate
	i32 _samplingRate;

    /// @brief All samples possible to use
    std::unordered_map<std::string, std::string> _samplePaths;

	/// @brief Samples that are loaded into memory
	std::unordered_map<std::string, std::vector<f32>> _cachedSamples;

	/// @brief Mutex
	std::mutex _mutex;

    /// @brief Reads the samples' directory and gets names of all usable samples
    void _loadSamplePaths();

	/// @brief Load sound file
	/// @param samplePath path to the file
	/// @brief info struct storing sound's metadata
	SNDFILE* _openFile(const std::string& samplePath, SF_INFO& info);

	/// @brief Close sound file
	/// @param file sound file
	void _closeFile(SNDFILE* file);

    /// @brief Loads a sample from given file
    /// @param sampleName name of the sample
    std::vector<f32> _loadSample(const std::string& sampleName);

public:
    /// @brief Constructor
    /// @param samplesDirectory directory where the samples are stored
	/// @param samplingRate synthesiser's sampling rate
    SampleManager(const std::string& samplesDirectory, i32 samplingRate);

	/// @brief Loads samples from new directory
	/// @param samplesDirectory directory where the samples are stored
	void changeSamplesDirectory(const std::string& samplesDirectory);

    /// @brief Get list of samples
    std::vector<std::string> getSampleNames();

	/// @brief Get a particular sample. Store it into cache if it's not stored already.
	/// @param sampleName name of the sample
	const std::vector<f32>& getSample(const std::string& sampleName);

	/// @brief Clear cached samples
	void clearCache();
};

}