include_guard()

include("cmake/libs/rtmidi.cmake")
include("cmake/libs/portaudio.cmake")


set(SOURCES
    src/oscillators/Oscillator.cpp
    src/oscillators/SineOscillator.cpp
)

add_executable(synth synth.cpp ${SOURCES})

target_include_directories(synth PUBLIC include ${portaudio_SOURCE_DIR}/include)
target_link_libraries(synth rtmidi portaudio portaudiocpp)