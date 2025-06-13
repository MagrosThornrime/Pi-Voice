#include <midi/Note.hpp>
#include <fmt/core.h>
#include <cmath>

namespace midi {

Note note[128];

namespace _details {

NotesInitializer::NotesInitializer() noexcept {
    constexpr char names[][3] {
        "C", "C#", "D", "D#",
        "E", "F", "F#", "G",
        "G#", "A", "A#", "B"
    };
    constexpr u32 namesSize = sizeof(names) / sizeof(names[0]);

    for(i32 i = 0; i != 128; ++i) {
        note[i].num = i;
        fmt::format_to_n(note[i].name, sizeof(note[i].name), "{}_{}", names[i % namesSize], i / namesSize - 1);
        note[i].freq = 440.f * std::powf(2.f, (i - 69.f) / float(namesSize));
    }
}
NotesInitializer notes;

}

}