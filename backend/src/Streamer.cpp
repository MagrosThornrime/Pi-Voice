#include <Streamer.h>

static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    /* Cast data passed through stream to our structure. */
    SawtoothOscillator *oscillator = (SawtoothOscillator*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer; /* Prevent unused variable warning. */

    for( i=0; i<framesPerBuffer; i++ )
    {
        float phase = oscillator->getNextPhase();
        *out = phase;
        out++;
        *out = phase
        out++;
    }
    return 0;
}

void Streamer::_initializeStream(){
    PaError err;
    err = Pa_OpenDefaultStream( &stream,
                               0,          /* no input channels */
                               2,          /* stereo output */
                               paFloat32,  /* 32 bit floating point output */
                               SAMPLE_RATE,
                               256,        /* frames per buffer, i.e. the number
                                                  of sample frames that PortAudio will
                                                  request from the callback. Many apps
                                                  may want to use
                                                  paFramesPerBufferUnspecified, which
                                                  tells PortAudio to pick the best,
                                                  possibly changing, buffer size.*/
                               patestCallback, /* this is your callback function */
                               &_saw ); //This is a pointer that will be passed to

    if( err != paNoError ){
        std::cerr << "PortAudio error: " << Pa_GetErrorText( err ) << std::endl;
    }
}

Streamer::Streamer(){
    PaError err;
    err = Pa_Initialize();
    if( err != paNoError )
    {
        std::cerr << "Pa_Initialize failed: " << Pa_GetErrorText( err );
    }
}

Streamer::~Streamer(){
    err = Pa_Terminate();
    if( err != paNoError ){
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
}

void Streamer::run(){
    err = Pa_StartStream( stream );
    if( err != paNoError ){
        std::cerr << "Error starting stream." << std::endl;
    }

    /* Sleep for several seconds. */
    Pa_Sleep(10*1000);

    err = Pa_StopStream( stream );
    if( err != paNoError ){
        std::cerr << "Error stopping stream." << std::endl;
    }
}