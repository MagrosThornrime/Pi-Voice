#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <Types.hpp>
#include <sndfile.h>
#include <mutex>


namespace fileio {
/// @brief Loads audio samples so they can be used later as oscillators
class SampleManager {
    /// @brief A directory where the samples are stored
    std::string _samplesDirectory;

	i32 _samplingRate;

    /// @brief All samples possible to use
    std::unordered_map<std::string, std::string> _samplePaths;

	std::unordered_map<std::string, std::vector<f32>> _cachedSamples;

	std::mutex _mutex;

    /// @brief Reads the samples' directory and gets names of all usable samples
    void _loadSamplePaths();

	SNDFILE* _openFile(const std::string& samplePath, SF_INFO& info);

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

	const std::vector<f32>& getSample(const std::string& sampleName);
	void removeFromCache(const std::string& sampleName);
};

}