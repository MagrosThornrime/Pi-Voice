include_guard()

include("cmake/libs/rtmidi.cmake")
include("cmake/libs/fmt.cmake")
include("cmake/libs/ranges.cmake")

file(GLOB_RECURSE MIDI_SRC "src/midi/*.cpp")

get_property(orig_pic GLOBAL PROPERTY POSITION_INDEPENDENT_CODE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "Enable PIC for midi" FORCE)
add_library(midi STATIC ${MIDI_SRC})
if(orig_pic)
  set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ${orig_pic})
else()
  unset(CMAKE_POSITION_INDEPENDENT_CODE CACHE)
endif()

target_include_directories(midi PUBLIC include ${rtmidi_SOURCE_DIR}/include)
target_link_libraries(midi PUBLIC rtmidi fmt::fmt range-v3::range-v3)