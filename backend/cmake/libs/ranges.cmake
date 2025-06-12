include_guard()

include("cmake/CPM.cmake")

CPMAddPackage(
    NAME range-v3
    GITHUB_REPOSITORY ericniebler/range-v3
    GIT_TAG 0.12.0
    OPTIONS
        "RANGES_BUILD_CALENDAR_EXAMPLE OFF"
        "RANGE_V3_TESTS OFF"
        "RANGE_V3_EXAMPLES OFF"
)

if(range-v3_ADDED)
    find_package(range-v3)
else()
    message(ERROR "range-v3 not added")
endif()