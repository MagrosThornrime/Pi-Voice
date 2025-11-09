include_guard()

include("cmake/CPM.cmake")

CPMAddPackage(
        NAME libsndfile
        GITHUB_REPOSITORY libsndfile/libsndfile
        GIT_TAG 1.2.2
        VERSION 1.2.2
        OPTIONS "BUILD_EXAMPLES OFF" "BUILD_SHARED_LIBS ON"
)

if(libsndfile_ADDED)
    find_package(libsndfile)
else()
    message(ERROR "libsndfile not added")
endif()