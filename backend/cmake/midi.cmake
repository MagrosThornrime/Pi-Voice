include_guard()

include("cmake/libs/rtmidi.cmake")
include("cmake/libs/fmt.cmake")
include("cmake/libs/ranges.cmake")

file(GLOB_RECURSE MIDI_SRC "src/midi/*.cpp")
add_library(midi STATIC ${MIDI_SRC})
target_include_directories(midi PUBLIC include ${rtmidi_SOURCE_DIR}/include)
target_link_libraries(midi PUBLIC rtmidi fmt::fmt range-v3::range-v3)