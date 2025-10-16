include_guard()

include("cmake/libs/rtmidi.cmake")
include("cmake/libs/portaudio.cmake")
include("cmake/libs/fmt.cmake")
include("cmake/midi.cmake")

file(GLOB_RECURSE SOURCES src/oscillators/*.cpp src/Voice.cpp src/VoiceManager.cpp)

add_executable(synth Synth.cpp ${SOURCES})

target_include_directories(synth PUBLIC include ${portaudio_SOURCE_DIR}/include)
target_link_libraries(synth rtmidi portaudio portaudiocpp fmt::fmt midi)