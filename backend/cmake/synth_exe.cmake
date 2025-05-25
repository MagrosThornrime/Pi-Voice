include_guard()

include("cmake/libs/portaudio.cmake")


set(SOURCES
        src/SawtoothOscillator.cpp
        src/Streamer.cpp
)

add_executable(synth synth.cpp ${SOURCES})
target_include_directories(synth PUBLIC include ${portaudio_SOURCE_DIR}/include)
target_link_libraries(synth portaudio)