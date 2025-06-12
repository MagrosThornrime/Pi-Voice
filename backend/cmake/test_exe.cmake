include_guard()

include("cmake/libs/rtmidi.cmake")
include("cmake/libs/portaudio.cmake")

add_executable(test test.cpp)

target_link_libraries(test rtmidi portaudio fmt::fmt)