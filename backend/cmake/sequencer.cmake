include_guard()

include("cmake/libs/fmt.cmake")
include("cmake/libs/ranges.cmake")
include("cmake/libs/portaudio.cmake")
include("cmake/libs/libsndfile.cmake")

file(GLOB_RECURSE SEQ_SRC "src/sequencer/*.cpp")

get_property(orig_pic GLOBAL PROPERTY POSITION_INDEPENDENT_CODE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "Enable PIC for sequencer" FORCE)
add_library(sequencer STATIC ${SEQ_SRC})
if(orig_pic)
  set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ${orig_pic})
else()
  unset(CMAKE_POSITION_INDEPENDENT_CODE CACHE)
endif()

target_include_directories(sequencer PUBLIC include ${rtmidi_SOURCE_DIR}/include
	${libsndfile_SOURCE_DIR}/include
)
target_link_libraries(sequencer PUBLIC
	fmt::fmt
	range-v3::range-v3
	portaudio
	portaudiocpp
)