#include <application/Synthesiser.hpp>

namespace application {
Synthesiser::Synthesiser(const std::string& recordingPath, i32 channels, i32 sampleRate) {
	_pipeline = std::make_shared<Pipeline>();
	_recorder = std::make_shared<fileio::Recorder>(recordingPath, channels, sampleRate);



}
}