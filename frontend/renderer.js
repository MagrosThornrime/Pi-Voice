// renderer.js

// Logging helper
function writeLog(message) {
    const logFilePath = 'log.txt'; // adjust path if needed
    const timestamp = new Date().toISOString();
    const logMessage = `${timestamp} - ${message}\n`;

    try {
        // Use Node fs only if available (preload provides Node)
        window.fs?.appendFileSync
            ? window.fs.appendFileSync(logFilePath, logMessage)
            : console.log("Log:", logMessage);
    } catch (err) {
        console.error("Failed to write log:", err);
    }
}

const container = document.getElementById('toPrint');
let midiGetterID;

// Render messages to DOM
function appendMessage(msg) {
    const p = document.createElement('p');
    p.textContent = msg;
    container.appendChild(p);
}

// Fetch and display MIDI messages
async function getMIDIMessages() {
    try {
        const messages = await window.midi.readMidi();
        messages.forEach(appendMessage);
    } catch (error) {
        writeLog(`Error retrieving MIDI messages: ${error}`);
        appendMessage(`Error retrieving MIDI messages: ${error.message}`);
    }
}

// Open MIDI port and start polling
async function openMIDIPort(port) {
    try {
        await window.midi.openReader(port);
        if (midiGetterID) clearInterval(midiGetterID);
        midiGetterID = setInterval(getMIDIMessages, 100);
        writeLog(`Opened MIDI port ${port}`);
    } catch (err) {
        writeLog(`Error opening port ${port}: ${err}`);
        appendMessage(`Error opening port ${port}: ${err.message}`);
    }
}

// Button handlers
document.getElementById("call-cpp0")?.addEventListener('click', () => openMIDIPort(0));
document.getElementById("call-cpp1")?.addEventListener('click', () => openMIDIPort(1));

// Cleanup on page unload
window.addEventListener('beforeunload', () => {
    if (midiGetterID) clearInterval(midiGetterID);
    window.midi.cleanup();
    writeLog("MIDI cleanup done on page unload");
});
