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
    find_package(portaudio)
else()
    message(ERROR "portaudio not added")
endif()