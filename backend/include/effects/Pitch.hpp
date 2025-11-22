#pragma once

#include "Types.hpp"
#include <portaudiocpp/PortAudioCpp.hxx>
#include <vector>

namespace effects {
    class Pitch: public portaudio::CallbackInterface {
    public:
        Pitch() = default;
        Pitch(const u32 channels, float pitch);

        int paCallbackFun(const void* inputBuffer, void* outputBuffer, unsigned long numFrames, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags) override;

    protected:
        struct ChannelState {
            std::vector<float> buffer;
            size_t writePos = 0;
            double readPos = 0.0;
        };

        std::vector<ChannelState> _state;
        u32 _channels;
        float _pitch = 1.0f;

        void _set(const u32 channels, float pitch);
    };
}