#include <fileio/FileRecorder.hpp>

namespace fileio{
FileRecorder::FileRecorder(const std::string& filename, unsigned channels, unsigned sampleRate, size_t capacityFrames)
        : _filename(filename),
        _channels(channels),
        _sampleRate(sampleRate),
        _capacityFrames(nextPowerOfTwo(std::max<size_t>(capacityFrames, 256))),
        _capacitySamples(_capacityFrames* channels),
        _mask(_capacitySamples - 1),
        _buffer(_capacitySamples) {}

FileRecorder::~FileRecorder() {
    stop(); // ensure writer stopped and file patched
}

bool FileRecorder::start() {
    if (_running.load(std::memory_order_acquire)) return false;
    _f = std::fopen(_filename.c_str(), "wb+");
    if (!_f) {
        std::perror("FileRecorder: open failed");
        return false;
    }
    if (!writeWavHeaderPlaceholder()) {
        std::fclose(_f); _f = nullptr;
        return false;
    }
    _running.store(true, std::memory_order_release);
    _writerThread = std::thread(&FileRecorder::writerThreadFunc, this);
    return true;
}

void FileRecorder::stop(){
    if (!_running.load(std::memory_order_acquire)) {
        if (_f) { std::fclose(_f); _f = nullptr; }
        return;
    }
    _running.store(false, std::memory_order_release);
    _cv.notify_one();
    if (_writerThread.joinable()) _writerThread.join();
    patchWavHeader();
    if (_f) { std::fclose(_f); _f = nullptr; }
}

int FileRecorder::paCallbackFun(const void* inputBuffer, void* /*outputBuffer*/, unsigned long numFrames,
        const PaStreamCallbackTimeInfo* /*timeInfo*/, PaStreamCallbackFlags statusFlags) {
    // Optionally track statusFlags (non-blocking update)
    if (statusFlags & paInputOverflow) {
        _statusInputOverflow.fetch_add(1, std::memory_order_relaxed);
    }
    if (statusFlags & paOutputUnderflow) {
        _statusOutputUnderflow.fetch_add(1, std::memory_order_relaxed);
    }

    const float* in = static_cast<const float*>(inputBuffer);
    const bool haveInput = (in != nullptr);

    const size_t samplesToPush = static_cast<size_t>(numFrames) * _channels;
    if (samplesToPush == 0) return paContinue;

    uint64_t w = _writeIndex.load(std::memory_order_relaxed);
    uint64_t r = _readIndex.load(std::memory_order_acquire);
    uint64_t available = w - r;
    uint64_t freeSpace = _capacitySamples - available;

    if (freeSpace == 0) {
        // ring full: drop full block
        _droppedSamples.fetch_add(samplesToPush, std::memory_order_relaxed);
        return paContinue;
    }
    const size_t toWrite = static_cast<size_t>(std::min<uint64_t>(samplesToPush, freeSpace));
    const size_t pos = static_cast<size_t>(w & _mask);

    // write in one or two memcpy calls to handle wrap
    const size_t first = std::min(toWrite, _capacitySamples - pos);
    if (haveInput) {
        std::memcpy(&_buffer[pos], in, first * sizeof(float));
        if (first < toWrite) {
            std::memcpy(&_buffer[0], in + first, (toWrite - first) * sizeof(float));
        }
    } else {
        // inputBuffer == nullptr -> write zeros
        std::memset(&_buffer[pos], 0, first * sizeof(float));
        if (first < toWrite) {
            std::memset(&_buffer[0], 0, (toWrite - first) * sizeof(float));
        }
    }

    _writeIndex.store(w + toWrite, std::memory_order_release);

    // cheap notify for writer thread; allowed in callback
    _cv.notify_one();

    return paContinue;
}

void FileRecorder::setFilename(const std::string& filename) {
    if (_running.load(std::memory_order_acquire)) return;
    _filename = filename;
}

uint64_t FileRecorder::droppedSamples() const {
    return _droppedSamples.load(std::memory_order_relaxed);
}

// Diagnostics
uint64_t FileRecorder::statusInputOverflowCount() const {
    return _statusInputOverflow.load();
}

uint64_t FileRecorder::statusOutputUnderflowCount() const {
    return _statusOutputUnderflow.load();
}

unsigned FileRecorder::channels() const {
    return _channels;
}

unsigned FileRecorder::sampleRate() const {
    return _sampleRate;
}

size_t FileRecorder::nextPowerOfTwo(size_t v) {
    size_t p = 1;
    while (p < v) p <<= 1;
    return p;
}

void FileRecorder::writerThreadFunc() {
    const size_t maxChunkSamples = 4096 * _channels;
    std::vector<float> tmp;
    tmp.reserve(maxChunkSamples);

    while (_running.load(std::memory_order_acquire) || (_readIndex.load(std::memory_order_relaxed) != _writeIndex.load(std::memory_order_acquire))) {
        uint64_t w = _writeIndex.load(std::memory_order_acquire);
        uint64_t r = _readIndex.load(std::memory_order_relaxed);
        uint64_t avail = w - r;
        if (avail == 0) {
            std::unique_lock<std::mutex> lk(_cvMutex);
            _cv.wait_for(lk, std::chrono::milliseconds(50));
            continue;
        }

        size_t chunk = static_cast<size_t>(std::min<uint64_t>(avail, maxChunkSamples));
        tmp.resize(chunk);

        size_t pos = static_cast<size_t>(r & _mask);
        size_t first = std::min(chunk, _capacitySamples - pos);
        std::memcpy(tmp.data(), &_buffer[pos], first * sizeof(float));
        if (first < chunk) {
            std::memcpy(tmp.data() + first, &_buffer[0], (chunk - first) * sizeof(float));
        }

        size_t written = std::fwrite(tmp.data(), sizeof(float), chunk, _f);
        if (written != chunk) {
            // log and continue; keep read index in sync for data we consumed
            std::perror("FileRecorder: fwrite failed");
        }
        _dataBytesWritten += written * sizeof(float);
        _readIndex.store(r + written, std::memory_order_release);
    }

    std::fflush(_f);
}

void FileRecorder::writeLE_u32(FILE* f, uint32_t v) {
    uint8_t buf[4];
    buf[0] = (uint8_t)(v & 0xff);
    buf[1] = (uint8_t)((v >> 8) & 0xff);
    buf[2] = (uint8_t)((v >> 16) & 0xff);
    buf[3] = (uint8_t)((v >> 24) & 0xff);
    std::fwrite(buf, 1, 4, f);
}

void FileRecorder::writeLE_u16(FILE* f, uint16_t v) {
    uint8_t buf[2];
    buf[0] = (uint8_t)(v & 0xff);
    buf[1] = (uint8_t)((v >> 8) & 0xff);
    std::fwrite(buf, 1, 2, f);
}

bool FileRecorder::writeWavHeaderPlaceholder() {
    // 32-bit float WAV (audioFormat = 3)
    std::fwrite("RIFF", 1, 4, _f);
    writeLE_u32(_f, 0); // placeholder chunk size
    std::fwrite("WAVE", 1, 4, _f);

    std::fwrite("fmt ", 1, 4, _f);
    writeLE_u32(_f, 16); // fmt chunk size
    writeLE_u16(_f, 3); // format = IEEE float
    writeLE_u16(_f, static_cast<uint16_t>(_channels));
    writeLE_u32(_f, static_cast<uint32_t>(_sampleRate));
    uint32_t byteRate = _sampleRate * _channels * sizeof(float);
    writeLE_u32(_f, byteRate);
    uint16_t blockAlign = static_cast<uint16_t>(_channels * sizeof(float));
    writeLE_u16(_f, blockAlign);
    writeLE_u16(_f, static_cast<uint16_t>(8 * sizeof(float)));

    std::fwrite("data", 1, 4, _f);
    writeLE_u32(_f, 0); // placeholder data size
    return true;
}

void FileRecorder::patchWavHeader() {
    if (!_f) return;
    uint64_t dataSize = _dataBytesWritten;
    uint64_t riffSize = 36 + dataSize;

    std::fseek(_f, 4, SEEK_SET);
    writeLE_u32(_f, static_cast<uint32_t>(riffSize));
    std::fseek(_f, 40, SEEK_SET);
    writeLE_u32(_f, static_cast<uint32_t>(dataSize));
    std::fflush(_f);
}
}