#include <fileio/SampleManager.hpp>
#include <filesystem>
#include <string>
#include <fmt/core.h>
#include <range/v3/all.hpp>
#include <Oscillators.hpp>

namespace fs = std::filesystem;

namespace fileio {
SampleManager::SampleManager(const std::string& samplesDirectory, i32 samplingRate)
: _samplesDirectory(samplesDirectory), _samplingRate(samplingRate) {
    _loadSamplePaths();
}

void SampleManager::changeSamplesDirectory(const std::string& samplesDirectory) {
	auto lock = std::lock_guard(_mutex);
	if (!_cachedSamples.empty()){
		throw std::runtime_error("Couldn't change samples directory: a sample is being used as oscillator");
	}
	_samplePaths.clear();
	_samplesDirectory = samplesDirectory;
	_loadSamplePaths();
}

SNDFILE* SampleManager::_openFile(const std::string& samplePath, SF_INFO& info) {
	info.format = 0;
	SNDFILE* file = sf_open(samplePath.c_str(), SFM_READ, &info);
	return file;
}

void SampleManager::_closeFile(SNDFILE* file) {
	sf_close(file);
}

void SampleManager::_loadSamplePaths() {
    for (const auto& entry : fs::directory_iterator(_samplesDirectory)) {
        fs::path entryPath = entry.path();
        std::string sampleName = entryPath.stem().string();
        if(!fs::is_regular_file(entryPath)) {
			fmt::println("Sample {} not loaded: not a regular file", sampleName);
            continue;
        }
		SF_INFO info;
		SNDFILE* file = _openFile(entryPath.string(), info);
		if(!file) {
			fmt::println("Sample {} not loaded: {}", sampleName, sf_strerror(file));
			continue;
		}
		if(info.samplerate != _samplingRate){
			fmt::println("Sample {} not loaded: it has sampling rate {}, but the app uses {}", sampleName,
				info.samplerate, _samplingRate);
			continue;
		}
		_closeFile(file);
        std:: string currentName = sampleName;
        int sameNamed = 1;
        while(_samplePaths.contains(currentName) || oscillators::reservedOscillators.contains(currentName)) {
            sameNamed++;
            currentName = fmt::format("{} {}", currentName, sameNamed);
        }
        _samplePaths.emplace(currentName, entryPath.string());
    }
}

std::vector<f32> SampleManager::_loadSample(const std::string& sampleName) {
	std::string samplePath;
	try{
		samplePath = _samplePaths.at(sampleName);
	} catch (const std::out_of_range& e) {
		throw std::invalid_argument(fmt::format("Invalid sample name: {}", sampleName));
	}
	SF_INFO info;
	SNDFILE* file = _openFile(samplePath, info);
    std::vector<f32> interleaved(info.frames * info.channels);
	i32 framesRead = sf_readf_float(file, interleaved.data(), info.frames);
	std::vector<f32> output(framesRead);
	for(i32 frame=0; frame < framesRead; frame++) {
		for(i32 channel=0; channel < info.channels; channel++){
			i32 index = info.channels * frame + channel;
			output[frame] += interleaved[index] / info.channels;
		}
	}
	_closeFile(file);
	return output;
}

const std::vector<f32>& SampleManager::getSample(const std::string& sampleName) {
	auto lock = std::lock_guard(_mutex);
	if(!_cachedSamples.contains(sampleName)) {
		_cachedSamples[sampleName] = _loadSample(sampleName);
	}
	return _cachedSamples.at(sampleName);
}

void SampleManager::clearCache() {
	auto lock = std::lock_guard(_mutex);
	_cachedSamples.clear();
}

std::vector<std::string> SampleManager::getSampleNames(){
	std::vector<std::string> names;
	for (const auto& entry : ranges::views::all(oscillators::reservedOscillators)) {
		names.emplace_back(entry);
	}
	for (const auto& entry : ranges::views::keys(_samplePaths)) {
		names.emplace_back(entry);
	}
	return names;
}

}