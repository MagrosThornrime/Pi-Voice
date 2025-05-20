include_guard()

include("cmake/libs/rtmidi.cmake")

add_library(MIDIReader SHARED)

target_sources(MIDIReader PRIVATE src/MIDIReader.cpp "${CMAKE_JS_SRC}")
target_include_directories(MIDIReader PUBLIC include "${CMAKE_JS_INC}")
target_link_libraries(MIDIReader rtmidi "${CMAKE_JS_LIB}")
set_target_properties(MIDIReader PROPERTIES PREFIX "" SUFFIX ".node")