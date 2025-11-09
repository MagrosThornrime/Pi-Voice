#include <fileio/SampleManager.hpp>
#include <filesystem>
#include <string>
#include <fmt/core.h>
#include <sys/stat.h>

namespace fs = std::filesystem;

namespace fileio {
SampleManager::SampleManager(const std::string& samplesDirectory)
: _samplesDirectory(samplesDirectory) {
    _loadSampleNames();
}

void SampleManager::_loadSampleNames() {
    // This structure would distinguish a file from a
    // directory
    struct stat sb;

    // Looping until all the items of the directory are
    // exhausted
    for (const auto& entry : fs::directory_iterator(_samplesDirectory)) {

        // Converting the path to const char * in the
        // subsequent lines
        std::filesystem::path outfilename = entry.path();
        std::string outfilename_str = outfilename.string();
        const char* path = outfilename_str.c_str();

        // Testing whether the path points to a
        // non-directory or not If it does, displays path
        if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR))
            fmt::println("{}", path);
    }
}
}