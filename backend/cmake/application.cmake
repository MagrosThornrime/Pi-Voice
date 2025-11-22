include_guard()

include("cmake/libs/fmt.cmake")
include("cmake/libs/ranges.cmake")
include("cmake/libs/portaudio.cmake")
include("cmake/fileio.cmake")

file(GLOB_RECURSE APPLICATION_SRC "src/application/*.cpp")

get_property(orig_pic GLOBAL PROPERTY POSITION_INDEPENDENT_CODE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "Enable PIC for pipeline" FORCE)
add_library(application STATIC ${APPLICATION_SRC})
if(orig_pic)
    set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ${orig_pic})
else()
    unset(CMAKE_POSITION_INDEPENDENT_CODE CACHE)
endif()

target_include_directories(application PUBLIC include ${rtmidi_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../frontend/node_modules/node-api-headers/include ${PROJECT_SOURCE_DIR}/../frontend/node_modules/node-addon-api)
target_link_libraries(application PUBLIC fmt::fmt range-v3::range-v3 portaudio portaudiocpp rtmidi fileio)