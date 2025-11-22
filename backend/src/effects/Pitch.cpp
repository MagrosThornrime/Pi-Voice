#include "effects/Pitch.hpp"
#include <cmath>

namespace effects {
    Pitch::Pitch(const u32 channels, float pitch) {
        _set(channels, pitch);
    }

    int Pitch::paCallbackFun(const void* inputBuffer, void* outputBuffer, unsigned long numFrames,
        const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags) {
        auto in = (const float*)inputBuffer;
        auto out = (float*)outputBuffer;

        const u32 bufferSize = 48000;

        u32 idx = 0;
        for (u32 i = 0; i < numFrames; ++i) {
            for (u32 ch = 0; ch < _channels; ++ch, ++idx) {
                auto&& st = _state[ch];
                auto& buf = st.buffer;

                buf[st.writePos] = in[idx];
                st.writePos = (st.writePos + 1) % bufferSize;

                size_t i0 = (u32)st.readPos % bufferSize;
                size_t i1 = (i0 + 1) % bufferSize;
                float frac = float(st.readPos - floor(st.readPos));

                float a = buf[i0];
                float b = buf[i1];
                float y = a + (b - a) * frac;

                st.readPos += _pitch;
                if (st.readPos >= bufferSize)
                    st.readPos -= bufferSize;

                out[idx] = y;
            }
        }

        return paContinue;
    }

    void Pitch::_set(const u32 channels, float pitch) {
        _channels = channels;
        _pitch = pitch;

        const u32 bufferSize = 48000;
        _state = decltype(_state)(channels);
        for (auto& st : _state) {
            st.buffer.assign(bufferSize, 0.0f);
            st.writePos = 0;
            st.readPos = 0.0;
        }
    }
}