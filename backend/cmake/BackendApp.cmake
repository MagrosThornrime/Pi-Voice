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
include("cmake/application.cmake")

add_library(BackendApp SHARED)

target_sources(BackendApp PRIVATE src/BackendApp.cpp "${CMAKE_JS_SRC}")
target_include_directories(BackendApp PUBLIC include "${CMAKE_JS_INC}" ${portaudio_SOURCE_DIR}/include)
target_link_libraries(BackendApp rtmidi "${CMAKE_JS_LIB}" portaudio fmt::fmt range-v3::range-v3
         -Wl,--whole-archive midi pipeline filters polyphonic oscillators fileio application  -Wl,--no-whole-archive )
set_target_properties(BackendApp PROPERTIES PREFIX "" SUFFIX ".node")