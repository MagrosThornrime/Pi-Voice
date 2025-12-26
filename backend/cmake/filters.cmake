include_guard()

include("cmake/libs/fmt.cmake")
include("cmake/libs/ranges.cmake")
include("cmake/libs/portaudio.cmake")

file(GLOB_RECURSE FILTERS_SRC "src/filters/*.cpp")

get_property(orig_pic GLOBAL PROPERTY POSITION_INDEPENDENT_CODE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "Enable PIC for filters" FORCE)
add_library(filters STATIC ${FILTERS_SRC})
if(orig_pic)
  set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ${orig_pic})
else()
  unset(CMAKE_POSITION_INDEPENDENT_CODE CACHE)
endif()

target_include_directories(filters PUBLIC include ${rtmidi_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../frontend/node_modules/node-api-headers/include ${PROJECT_SOURCE_DIR}/../frontend/node_modules/node-addon-api/include)
target_link_libraries(filters PUBLIC fmt::fmt range-v3::range-v3 portaudio portaudiocpp)