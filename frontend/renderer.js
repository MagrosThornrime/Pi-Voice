let addon;
try {
    addon = require('./lib/MIDIReader');
} 
catch (e) {
    console.error("Failed to load the MIDIReader module. Ensure it is built correctly.");
    console.error(e);
    process.exit(1);
}

const getMIDIMessages = () => {
    try{
        console.log("Retrieving MIDI messages...");
        const messages = addon.getMIDI();
        console.log("MIDI messages retrieved:", messages);
        const container = document.getElementById('toPrint');
        messages.forEach(msg => {
            const newP = document.createElement('p');
            newP.textContent = msg;
            container.appendChild(newP);
            if (msg == "<NO-DEVICE>") {
                clearInterval(midiGetterID);
            }
        });
    } catch (error) {
        console.error("Error retrieving MIDI messages:", error);
        const container = document.getElementById('toPrint');
        const newP = document.createElement('p');
        newP.textContent = "Error retrieving MIDI messages: " + error.message;
        container.appendChild(newP);
    }
};

const midiGetterID = setInterval(getMIDIMessages, 1000);

function cleanup() {
    midiModule.cleanup();
    process.exit(0);
}

process
    .on('SIGINT', cleanup)
    .on('SIGTERM', cleanup)
    .on('exit', () => {
        midiModule.cleanup();
    });
