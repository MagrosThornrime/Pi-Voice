include_guard()

include("cmake/CPM.cmake")

CPMAddPackage(
        NAME googletest
        GITHUB_REPOSITORY google/googletest
        GIT_TAG release-1.12.1
        VERSION 1.12.1
        OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
)

if(googletest_ADDED)
    find_package(googletest)
else()
    message(ERROR "googletest not added")
endif()