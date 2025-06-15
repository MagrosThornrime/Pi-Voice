cmake_minimum_required(VERSION 3.25)

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

include_directories("include")

include("cmake/MidiTest.cmake")
include("cmake/Synth.cmake")
include("cmake/MIDIReader.cmake")
include("cmake/Tests.cmake")