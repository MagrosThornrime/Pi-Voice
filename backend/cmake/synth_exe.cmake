include_guard()

include("cmake/libs/portaudio.cmake")

set(HEADERS
        include/SawtoothOscillator.hpp
        include/Streamer.hpp
)

set(SOURCES
        src/SawtoothOscillator.cpp
        src/Streamer.cpp
)

add_executable(synth synth.cpp ${HEADERS} ${SOURCES})
target_link_directories(synth PRIVATE include)
target_link_libraries(synth portaudio)