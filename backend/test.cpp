#include <fmt/core.h>
#include <RtMidi.h>

int main() {
    RtMidiIn midiIn;
    std::vector<unsigned char> msg;
    int n;
    int i;
    double stamp;

    n = midiIn.getPortCount();
    if (n == 0) {
        fmt::println("No ports");
        return 1;
    }

    midiIn.openPort(0);
    midiIn.ignoreTypes(false, false, false);
}