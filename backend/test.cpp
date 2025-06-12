#include <iostream>
#include <RtMidi.h>

int main() {
    RtMidiIn midiIn;
    std::vector<unsigned char> msg;
    int n;
    int i;
    double stamp;

    n = midiIn.getPortCount();
    if (n == 0) {
		std::cout << "No ports" << std::endl;
        return 1;
    }

    midiIn.openPort(0);
    midiIn.ignoreTypes(false, false, false);
}