include_guard()

include("cmake/libs/rtmidi.cmake")
include("cmake/libs/portaudio.cmake")
include("cmake/libs/googletest.cmake")

enable_testing()

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

foreach(TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SRC})

    target_include_directories(${TEST_NAME} PUBLIC include ${portaudio_SOURCE_DIR}/include)
    target_link_libraries(${TEST_NAME}
            PRIVATE
            rtmidi
            portaudio
            portaudiocpp
            gtest
            gtest_main
            $<$<NOT:$<PLATFORM_ID:Windows>>:pthread>
    )

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
