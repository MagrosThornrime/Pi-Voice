cmake_minimum_required(VERSION 3.28)

project(audio_platform_backend)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE "Release")

if(MSVC)
    add_compile_options("/permissive-")
    add_compile_options("/Zc:__cplusplus")
    add_compile_options("/Zc:preprocessor")
endif()

include_directories("../frontend/node_modules/node-addon-api")
include_directories("../frontend/node_modules/node-api-headers/include")

include("cmake/test_exe.cmake")
include("cmake/synth_exe.cmake")
include("cmake/MIDIReader.cmake")
