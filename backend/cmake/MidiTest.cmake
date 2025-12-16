include_guard()

include("cmake/libs/rtmidi.cmake")
include("cmake/libs/portaudio.cmake")
include("cmake/midi.cmake")

add_executable(miditest MidiTest.cpp)

target_link_libraries(miditest rtmidi portaudio fmt::fmt midi)