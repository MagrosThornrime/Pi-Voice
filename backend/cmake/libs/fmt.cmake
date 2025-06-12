include_guard()

include("cmake/CPM.cmake")

CPMAddPackage(
    NAME fmt
    GITHUB_REPOSITORY fmtlib/fmt
    GIT_TAG 11.2.0
    OPTIONS
        "FMT_SHARED=ON"
        "FMT_DOC=OFF"
        "FMT_TEST=OFF"
)

if(fmt_ADDED)
    find_package(fmt)
else()
    message(ERROR "fmt not added")
endif()