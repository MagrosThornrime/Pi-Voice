include_guard()

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
include("cmake/application.cmake")
include("cmake/effects.cmake")

add_executable(ConsoleApp src/ConsoleApp.cpp)

if (MSVC)
    target_link_libraries(ConsoleApp
            PRIVATE
            rtmidi
            sndfile
            portaudio
            fmt::fmt
            range-v3::range-v3
            midi
            pipeline
            filters
            polyphonic
            oscillators
            fileio
            application
    )
    target_link_options(ConsoleApp PRIVATE
            /WHOLEARCHIVE:midi
            /WHOLEARCHIVE:pipeline
            /WHOLEARCHIVE:filters
            /WHOLEARCHIVE:polyphonic
            /WHOLEARCHIVE:oscillators
            /WHOLEARCHIVE:fileio
            /WHOLEARCHIVE:application
    )
else()
    target_link_libraries(ConsoleApp
            PRIVATE
            rtmidi
            sndfile
            portaudio
            fmt::fmt
            range-v3::range-v3
            -Wl,--whole-archive
            midi
            pipeline
            filters
            polyphonic
            oscillators
            fileio
            application
            -Wl,--no-whole-archive
    )
endif()