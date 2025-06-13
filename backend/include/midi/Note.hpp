#pragma once

#include <Types.hpp>
#include <string_view>

namespace midi {

namespace _details {

struct NotesInitializer {
    NotesInitializer() noexcept;
};
extern NotesInitializer notes;

}

struct Note {
    i32 num;
    char name[8];
    float freq;
};
extern Note note[128];

}