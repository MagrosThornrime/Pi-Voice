#include <fileio/SampleManager.hpp>
#include <filesystem>
#include <string>
#include <fmt/core.h>
#include <range/v3/all.hpp>

namespace fs = std::filesystem;

namespace fileio {
SampleManager::SampleManager(const std::string& samplesDirectory)
: _samplesDirectory(samplesDirectory) {
    _loadSamplePaths();
}

std::vector<std::string> SampleManager::getSampleNames() {
    std::vector<std::string> names;
    for (const auto& sample: ranges::views::keys(_samplePaths)) {
        names.push_back(sample);
    }
    return names;
}


void SampleManager::_loadSamplePaths() {
    for (const auto& entry : fs::directory_iterator(_samplesDirectory)) {
        fs::path entryPath = entry.path();
        if(!fs::is_regular_file(entryPath)) {
            continue;
        }
        std::string sampleName = entryPath.stem().string();
        std:: string currentName = sampleName;
        int sameNamed = 1;
        while(_samplePaths.contains(currentName)) {
            sameNamed++;
            currentName = fmt::format("{} {}", currentName, sameNamed);
        }
        _samplePaths.emplace(currentName, entryPath.string());
    }
}
}