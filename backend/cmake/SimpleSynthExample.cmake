include_guard()

include("cmake/libs/rtmidi.cmake")
include("cmake/libs/portaudio.cmake")
include("cmake/libs/fmt.cmake")
include("cmake/midi.cmake")
include("cmake/pipeline.cmake")
include("cmake/filters.cmake")
include("cmake/polyphonic.cmake")
include("cmake/oscillators.cmake")
include("cmake/fileio.cmake")

add_library(SimpleSynthExample SHARED)

target_sources(SimpleSynthExample PRIVATE src/SimpleSynthExample.cpp "${CMAKE_JS_SRC}")
target_include_directories(SimpleSynthExample PUBLIC include "${CMAKE_JS_INC}" ${portaudio_SOURCE_DIR}/include)
target_link_libraries(SimpleSynthExample rtmidi "${CMAKE_JS_LIB}" portaudio fmt::fmt range-v3::range-v3
        midi pipeline filters polyphonic oscillators fileio)
set_target_properties(SimpleSynthExample PROPERTIES PREFIX "" SUFFIX ".node")