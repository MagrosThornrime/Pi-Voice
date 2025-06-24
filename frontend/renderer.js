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
    addon = require('./lib/SimpleMIDIReaderExample.node');
}
catch (e) {
    writeLog("Failed to load the SimpleMIDIReaderExample module. Ensure it is built correctly.");
    writeLog(e);
    exit(1);
}

const container = document.getElementById('toPrint');

addon.midiPorts().forEach(element => {
	writeLog(`${element}`);
	const newP = document.createElement('p');
	newP.textContent = `${element}`
	container.appendChild(newP);
});

const getMIDIMessages = () => {
    try{
        writeLog("Retrieving MIDI messages...");
        const messages = addon.readMidi();
        writeLog(`MIDI messages retrieved: ${messages}`);
        messages.forEach(msg => {
            const newP = document.createElement('p');
            newP.textContent = msg;
            container.appendChild(newP);
        });
    } catch (error) {
        writeLog(`Error retrieving MIDI messages: ${error}`);
        const container = document.getElementById('toPrint');
        const newP = document.createElement('p');
        newP.textContent = "Error retrieving MIDI messages: " + error.message;
        container.appendChild(newP);
    }
};
var midiGetterID;

document.getElementById("call-cpp0").onclick = () => {
	try{
		addon.openReader(0);
		midiGetterID = setInterval(getMIDIMessages, 100);
	}
	catch(err) {
        writeLog(`Error opening port 0: ${err}`);
        const newP = document.createElement('p');
        newP.textContent = `Error opening port 0: ${err.message}`;
        container.appendChild(newP);
	}
}
document.getElementById("call-cpp1").onclick = () => {
	try{
		addon.openReader(1);
		midiGetterID = setInterval(getMIDIMessages, 100);
	}
	catch(err) {
        writeLog(`Error opening port 1: ${err}`);
        const newP = document.createElement('p');
        newP.textContent = `Error opening port 1: ${err.message}`;
        container.appendChild(newP);
	}
}

function cleanup() {
    addon.cleanup();
    process.exit(0);
}

process
    .on('SIGINT', cleanup)
    .on('SIGTERM', cleanup)
    .on('exit', () => {
        addon.cleanup();
    });
