include_guard()

include("cmake/libs/rtmidi.cmake")
include("cmake/libs/portaudio.cmake")

add_executable(miditest src/MidiTest.cpp)

target_link_libraries(miditest rtmidi portaudio fmt::fmt)