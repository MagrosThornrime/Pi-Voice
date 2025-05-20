include_guard()

include("cmake/CPM.cmake")

CPMAddPackage(
    NAME rtmidi
    URL http://www.music.mcgill.ca/~gary/rtmidi/release/rtmidi-6.0.0.tar.gz
    VERSION 6.0.0
    OPTIONS
    "BUILD_SHARED_LIBS OFF"
    "RTMIDI_BUILD_TESTING OFF"
)

if(rtmidi_ADDED)
    find_package(rtmidi)
else()
    message(ERROR "rtmidi not added")
endif()