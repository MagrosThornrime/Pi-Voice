include_guard()

include("cmake/CPM.cmake")

CPMAddPackage(
  NAME portaudio
  GITHUB_REPOSITORY PortAudio/portaudio
  GIT_TAG v19.7.0
  OPTIONS
    "PA_BUILD_SHARED ON"
    "PA_USE_ASIO OFF"
)

if(portaudio_ADDED)
    find_package(portaudio REQUIRED)

    set(portaudiocpp_SRC_DIR ${portaudio_SOURCE_DIR}/bindings/cpp)

    file(GLOB_RECURSE portaudiocpp_sources
            ${portaudiocpp_SRC_DIR}/source/*.cxx
    )

    add_library(portaudiocpp ${portaudiocpp_sources})

    target_include_directories(portaudiocpp PUBLIC
            ${portaudiocpp_SRC_DIR}/include
    )

    target_link_libraries(portaudiocpp PUBLIC portaudio)

else()
    message(FATAL_ERROR "portaudio not added")
endif()
