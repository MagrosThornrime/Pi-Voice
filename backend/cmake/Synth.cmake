include_guard()

include("cmake/libs/rtmidi.cmake")
include("cmake/libs/portaudio.cmake")

file(GLOB_RECURSE SOURCES src/oscillators/*.cpp)

add_executable(synth Synth.cpp ${SOURCES})

target_include_directories(synth PUBLIC include ${portaudio_SOURCE_DIR}/include)
target_link_libraries(synth rtmidi portaudio portaudiocpp)