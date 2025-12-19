#include_guard()
#
#include("cmake/libs/rtmidi.cmake")
#include("cmake/libs/portaudio.cmake")
#include("cmake/libs/fmt.cmake")
#include("cmake/libs/libsndfile.cmake")
#include("cmake/midi.cmake")
#include("cmake/pipeline.cmake")
#include("cmake/filters.cmake")
#include("cmake/polyphonic.cmake")
#include("cmake/oscillators.cmake")
#include("cmake/fileio.cmake")
#include("cmake/application.cmake")
#
#target_include_directories(Main PUBLIC include "${CMAKE_JS_INC}" "${portaudio_SOURCE_DIR}/include" ${PROJECT_SOURCE_DIR}/../frontend/node_modules/node-api-headers/include ${PROJECT_SOURCE_DIR}/../frontend/node_modules/node-addon-api)
#target_sources(Main PRIVATE src/Main.cpp "${CMAKE_JS_SRC}")
#
#if (MSVC)
#    target_link_libraries(Main
#            PRIVATE
#            rtmidi
#            sndfile
#            "${CMAKE_JS_LIB}"
#            portaudio
#            fmt::fmt
#            range-v3::range-v3
#            midi
#            pipeline
#            filters
#            polyphonic
#            oscillators
#            fileio
#            application
#    )
#    target_link_options(Main PRIVATE
#            /WHOLEARCHIVE:midi
#            /WHOLEARCHIVE:pipeline
#            /WHOLEARCHIVE:filters
#            /WHOLEARCHIVE:polyphonic
#            /WHOLEARCHIVE:oscillators
#            /WHOLEARCHIVE:fileio
#            /WHOLEARCHIVE:application
#    )
#else()
#    target_link_libraries(Main
#            PRIVATE
#            rtmidi
#            sndfile
#            "${CMAKE_JS_LIB}"
#            portaudio
#            fmt::fmt
#            range-v3::range-v3
#            -Wl,--whole-archive
#            midi
#            pipeline
#            filters
#            polyphonic
#            oscillators
#            fileio
#            application
#            -Wl,--no-whole-archive
#    )
#endif()