include_guard()

include("cmake/libs/rtmidi.cmake")

add_executable(test test.cpp)

target_link_libraries(test rtmidi)