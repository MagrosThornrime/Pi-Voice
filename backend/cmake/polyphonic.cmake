include_guard()

include("cmake/libs/fmt.cmake")
include("cmake/libs/ranges.cmake")
include("cmake/libs/portaudio.cmake")

file(GLOB_RECURSE POLYPHONIC_SRC "src/polyphonic/*.cpp")

get_property(orig_pic GLOBAL PROPERTY POSITION_INDEPENDENT_CODE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "Enable PIC for polyphonic" FORCE)
add_library(polyphonic STATIC ${POLYPHONIC_SRC})
if(orig_pic)
    set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ${orig_pic})
else()
    unset(CMAKE_POSITION_INDEPENDENT_CODE CACHE)
endif()

target_include_directories(polyphonic PUBLIC include ${rtmidi_SOURCE_DIR}/include)
target_link_libraries(polyphonic PUBLIC fmt::fmt range-v3::range-v3 portaudio portaudiocpp)