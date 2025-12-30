include_guard()

include("cmake/libs/googletest.cmake")
include("cmake/libs/rtmidi.cmake")
include("cmake/libs/portaudio.cmake")
include("cmake/libs/fmt.cmake")
include("cmake/libs/libsndfile.cmake")
include("cmake/midi.cmake")
include("cmake/pipeline.cmake")
include("cmake/filters.cmake")
include("cmake/polyphonic.cmake")
include("cmake/oscillators.cmake")
include("cmake/fileio.cmake")
include("cmake/sequencer.cmake")

enable_testing()

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)


foreach(TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SRC})

    target_include_directories(${TEST_NAME} PUBLIC include ${portaudio_SOURCE_DIR}/include)
	if(MSVC)
		target_link_libraries(${TEST_NAME}
				PRIVATE
				rtmidi
				sndfile
				portaudio
				portaudiocpp
				fmt::fmt
				range-v3::range-v3
				gtest
				gtest_main
				midi
				pipeline
				filters
				polyphonic
				oscillators
				fileio
				sequencer
		)
		target_link_options(BackendApp PRIVATE
			/WHOLEARCHIVE:midi
			/WHOLEARCHIVE:pipeline
			/WHOLEARCHIVE:filters
			/WHOLEARCHIVE:polyphonic
			/WHOLEARCHIVE:oscillators
			/WHOLEARCHIVE:fileio
		)
	else()
		target_link_libraries(${TEST_NAME}
				PRIVATE
				rtmidi
				sndfile
				portaudio
				portaudiocpp
				fmt::fmt
				range-v3::range-v3
				gtest
				gtest_main
				$<$<NOT:$<PLATFORM_ID:Windows>>:pthread>
				-Wl,--whole-archive
				midi
				pipeline
				filters
				polyphonic
				oscillators
				fileio
				sequencer
				-Wl,--no-whole-archive
		)
	endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
