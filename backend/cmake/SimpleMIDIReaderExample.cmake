include_guard()

include("cmake/libs/rtmidi.cmake")
include("cmake/libs/portaudio.cmake")
include("cmake/libs/fmt.cmake")
include("cmake/libs/ranges.cmake")
include("cmake/midi.cmake")

add_library(SimpleMIDIReaderExample SHARED)

target_sources(SimpleMIDIReaderExample PRIVATE src/SimpleMIDIReaderExample.cpp "${CMAKE_JS_SRC}")
target_include_directories(SimpleMIDIReaderExample PUBLIC include "${CMAKE_JS_INC}" ${portaudio_SOURCE_DIR}/include)
target_link_libraries(SimpleMIDIReaderExample rtmidi "${CMAKE_JS_LIB}" portaudio fmt::fmt range-v3::range-v3 midi)
set_target_properties(SimpleMIDIReaderExample PROPERTIES PREFIX "" SUFFIX ".node")