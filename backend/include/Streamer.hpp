#pragma once
#include <portaudio.h>

/* This routine will be called by the PortAudio engine when audio is needed.
 * It may called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData );

class Streamer {
    PaStream* _stream;
    SawtoothOscillator _saw(-5.0f, 5.0f, 0.01f);

    void _initializeStream();

public:
    Streamer();
    Streamer( const Streamer& ) = delete;
    ~Streamer();
    void run();

};