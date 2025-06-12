include_guard()

include("cmake/libs/rtmidi.cmake")
include("cmake/libs/portaudio.cmake")
include("cmake/libs/fmt.cmake")
include("cmake/libs/ranges.cmake")

add_library(MIDIReader SHARED)

target_sources(MIDIReader PRIVATE src/MIDIReader.cpp "${CMAKE_JS_SRC}")
target_include_directories(MIDIReader PUBLIC include "${CMAKE_JS_INC}" ${portaudio_SOURCE_DIR}/include)
target_link_libraries(MIDIReader rtmidi "${CMAKE_JS_LIB}" portaudio fmt::fmt range-v3::range-v3)
set_target_properties(MIDIReader PROPERTIES PREFIX "" SUFFIX ".node")