include_guard()

include("cmake/CPM.cmake")

set(FMT_POSITION_INDEPENDENT_CODE ON CACHE BOOL "" FORCE)
CPMAddPackage("gh:fmtlib/fmt#11.2.0")

if(fmt_ADDED)
    find_package(fmt)
else()
    message(ERROR "fmt not added")
endif()