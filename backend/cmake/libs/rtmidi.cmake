include_guard()

include("cmake/CPM.cmake")

CPMAddPackage(
    NAME rtmidi
<<<<<<< HEAD
    URL https://github.com/thestk/rtmidi/archive/refs/tags/6.0.0.tar.gz    VERSION 6.0.0
=======
    URL https://github.com/thestk/rtmidi/archive/6.0.0/rtmidi-6.0.0.tar.gz
    VERSION 6.0.0
>>>>>>> chris/pipeline
    OPTIONS
    "BUILD_SHARED_LIBS ON"
    "RTMIDI_BUILD_TESTING OFF"
)

if(rtmidi_ADDED)
    find_package(rtmidi)
else()
    message(ERROR "rtmidi not added")
endif()