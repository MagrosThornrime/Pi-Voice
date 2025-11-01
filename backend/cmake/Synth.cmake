include_guard()

include("cmake/libs/rtmidi.cmake")
include("cmake/libs/portaudio.cmake")
include("cmake/libs/fmt.cmake")
include("cmake/midi.cmake")
include("cmake/pipeline.cmake")
include("cmake/filters.cmake")
include("cmake/polyphonic.cmake")
include("cmake/oscillators.cmake")

add_executable(synth Synth.cpp)

target_include_directories(synth PUBLIC include ${portaudio_SOURCE_DIR}/include)
target_link_libraries(synth rtmidi portaudio portaudiocpp fmt::fmt midi pipeline filters polyphonic oscillators)