const addon = require('./lib/MIDIReader');

const midiGetterID = setInterval(() => {
    const messages = addon.getMIDI();
    const container = document.getElementById('toPrint');
    messages.forEach(msg => {
        const newP = document.createElement('p');
        newP.textContent = msg;
        container.appendChild(newP);
        if (msg == "<NO-DEVICE>") {
            clearInterval(midiGetterID);
        }
    });

}, 100);

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
