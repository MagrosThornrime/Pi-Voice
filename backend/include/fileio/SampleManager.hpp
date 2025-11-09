#pragma once
#include <unordered_set>
#include <vector>
#include <string>
#include <Types.hpp>


namespace fileio {
/// @brief Loads audio samples so they can be used later as oscillators
class SampleManager {
    /// @brief A directory where the samples are stored
    std::string _samplesDirectory;

    /// @brief All samples possible to use
    std::unordered_set<std::string> _sampleNames;

    /// @brief Reads the samples' directory and gets names of all usable samples
    void _loadSampleNames();

public:
    /// @brief Constructor
    /// @param samplesDirectory a directory where the samples are stored
    SampleManager(const std::string& samplesDirectory);

    /// @brief Get list of samples
    std::vector<std::string> getSampleNames();

    /// @brief Loads a sample from given file
    /// @param sampleName name of the sample
    //std::vector<f32> loadSample(const std::string& sampleName);
};

}