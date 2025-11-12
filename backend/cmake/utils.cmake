include_guard()

include("cmake/libs/fmt.cmake")
include("cmake/libs/ranges.cmake")

file(GLOB_RECURSE UTILS_SRC "src/utils/*.cpp")

get_property(orig_pic GLOBAL PROPERTY POSITION_INDEPENDENT_CODE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "Enable PIC for utils" FORCE)
add_library(utils STATIC ${UTILS_SRC})
if(orig_pic)
  set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ${orig_pic})
else()
  unset(CMAKE_POSITION_INDEPENDENT_CODE CACHE)
endif()

target_include_directories(utils PUBLIC include ${rtmidi_SOURCE_DIR}/include
	${libsndfile_SOURCE_DIR}/include
)
target_link_libraries(utils PUBLIC
	fmt::fmt
	range-v3::range-v3
)