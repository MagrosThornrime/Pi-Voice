include_guard()

include("cmake/CPM.cmake")

CPMAddPackage(
    NAME rtmidi
    URL https://github.com/thestk/rtmidi/archive/refs/tags/6.0.0.tar.gz    VERSION 6.0.0
    OPTIONS
    "BUILD_SHARED_LIBS ON"
    "RTMIDI_BUILD_TESTING OFF"
)

if(rtmidi_ADDED)
    find_package(rtmidi)
else()
    message(ERROR "rtmidi not added")
endif()