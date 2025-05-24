const fs = require('fs');
const path = require('path');
const { exit } = require('process');

function writeLog(message) {
    const logFilePath = path.join(__dirname, 'log.txt');
    const timestamp = new Date().toISOString();
    const logMessage = `${timestamp} - ${message}\n`;
    
    fs.appendFileSync(logFilePath, logMessage, (err) => {
        if (err) {
            console.error("Failed to write to log file:", err);
        }
    });
}

let addon;
try {
    addon = require('./lib/MIDIReader');
} 
catch (e) {
    writeLog("Failed to load the MIDIReader module. Ensure it is built correctly.");
    writeLog(e);
    exit(1);
}

const getMIDIMessages = () => {
    try{
        writeLog("Retrieving MIDI messages...");
        const messages = addon.getMIDI();
        writeLog(`MIDI messages retrieved: ${messages}`);
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
        writeLog(`Error retrieving MIDI messages: ${error}`);
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
