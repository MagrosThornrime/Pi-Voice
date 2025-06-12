include_guard()

include("cmake/CPM.cmake")

CPMAddPackage("gh:fmtlib/fmt#11.2.0")

if(fmt_ADDED)
    find_package(fmt)
else()
    message(ERROR "fmt not added")
endif()