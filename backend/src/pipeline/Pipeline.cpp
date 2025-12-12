#include <pipeline/Pipeline.hpp>
#include <fmt/core.h>
#include <algorithm>

namespace pipeline {

using LayerRef = std::shared_ptr<Layer>;

// CHANGED: Constructor unchanged semantics but note we do NOT use an atomic _running anymore.
Pipeline::Pipeline(u32 framesPerCall, u32 channels,
                   std::shared_ptr<polyphonic::VoiceManager> voiceManager,
                   std::shared_ptr<fileio::FileRecorder> recorder)
    : _channels(channels),
      _outputQueue(framesPerCall * channels * 4),
      _voiceManager(std::move(voiceManager)),
      _recorder(std::move(recorder))
{
    // start producer thread; stop token provided automatically by jthread
    _producerThread = std::jthread(&Pipeline::_generateSound, this, framesPerCall);
    // CHANGED: removed _running atomic usage
}

Pipeline::~Pipeline() {
    if (_producerThread.joinable()) {
        _producerThread.request_stop();
        _producerThread.join();
    }
    // CHANGED: no _running to clear
}

// CHANGED: Lock is acquired first; handle sentinel safely.
// Default param still uses -1 sentinel; normalized inside function.
LayerRef Pipeline::add(const LayerRef& layer, u32 i) {
    if (!layer) {
        return nullptr;
    }

    std::unique_lock lock(_layersMutex); // CHANGED: acquire unique lock

    const u32 size = static_cast<u32>(_layers.size());
    // Treat u32(-1) as append; clamp out of range to append
    if (i == static_cast<u32>(-1) || i > size) {
        i = size; // insert at end (begin() + size == end())
    }
    // Now safe to insert at [0..size]
    _layers.insert(_layers.begin() + i, layer);
    return layer;
}

LayerRef Pipeline::remove(const u32 i) {
    std::unique_lock lock(_layersMutex); // CHANGED: lock first
    if (i >= _layers.size()) {
        return nullptr;
    }

    LayerRef result = std::move(_layers[i]);
    _layers.erase(_layers.begin() + i);
    return result;
}

void Pipeline::move(const u32 curr, const u32 target) {
    std::unique_lock lock(_layersMutex); // CHANGED: single lock to make move atomic
    const u32 n = static_cast<u32>(_layers.size());
    if (curr >= n) return;

    LayerRef item = std::move(_layers[curr]);
    _layers.erase(_layers.begin() + curr);

    u32 insertIndex = target;
    if (insertIndex == static_cast<u32>(-1) || insertIndex > _layers.size()) {
        insertIndex = static_cast<u32>(_layers.size());
    }
    _layers.insert(_layers.begin() + insertIndex, std::move(item));
}

void Pipeline::swap(const u32 i1, const u32 i2) {
    std::unique_lock lock(_layersMutex); // CHANGED: lock first
    if (i1 >= _layers.size() || i2 >= _layers.size()) return;
    std::swap(_layers[i1], _layers[i2]);
}

LayerRef Pipeline::get(const u32 i) const {
    std::unique_lock lock(_layersMutex); // CHANGED: lock first
    if (i >= _layers.size()) return nullptr;
    return _layers[i];
}

u32 Pipeline::length() const {
    std::unique_lock lock(_layersMutex); // CHANGED: lock to read size safely
    return static_cast<u32>(_layers.size());
}

// CHANGED: This callback now acquires the mutex to protect _layers/_outputQueue access.
// NOTE: locking in the audio callback may affect latency; consider snapshotting if you need ultra low latency.
int Pipeline::paCallbackFun(const void* /*inputBuffer*/, void* outputBuffer, unsigned long numFrames,
                            const PaStreamCallbackTimeInfo* /*timeInfo*/, PaStreamCallbackFlags /*statusFlags*/)
{
    std::unique_lock lock(_layersMutex); // CHANGED: protect reading queue and recorder

    float* out = static_cast<float*>(outputBuffer);
    const u32 samplesNeeded = static_cast<u32>(numFrames) * _channels;

    for (u32 i = 0; i < samplesNeeded; ++i) {
        float sample;
        if (!_outputQueue.pop(sample)) {
            sample = 0.0f;
        }
        out[i] = sample;
    }

    // Recorder API: assume it's thread-safe or expects to be called from audio thread
    if (_recorder) {
        // CHANGED: write() assumed to be safe from audio thread
        _recorder->write(out, numFrames);
    }

    return paContinue;
}

// CHANGED: _generateSound uses temp buffer and unique_lock while processing layers.
// Note: locking here serializes with paCallbackFun and add/remove/etc.
void Pipeline::_generateSound(std::stop_token stopToken, u32 framesPerCall)
{
    const u32 samplesPerCall = framesPerCall * _channels;
    std::vector<float> tempBuffer(samplesPerCall);

    while (!stopToken.stop_requested()) {
        // Fill buffer from voice manager (vector-based API)
        _voiceManager->generateSound(tempBuffer, framesPerCall);

        // CHANGED: use unique_lock instead of shared_lock (protect _layers while we iterate/process)
        {
            std::unique_lock lock(_layersMutex);
            for (auto&& layer : _layers) {
                // processSound(inputVector, outputVector, framesPerCall)
                // Here we use tempBuffer as input and output; layers must support in-place processing
                layer->processSound(tempBuffer, tempBuffer, framesPerCall);
            }
        }

        // push samples into output queue
        for (u32 i = 0; i < samplesPerCall; ++i) {
            // CHANGED: busy-wait yielding if queue full; this is as before
            while (!_outputQueue.push(tempBuffer[i])) {
                std::this_thread::yield();
            }
        }
    }
}

} // namespace pipeline
