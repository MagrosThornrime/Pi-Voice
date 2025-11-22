const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("synthAPI", {
    // MIDI
    ports: () => ipcRenderer.invoke("synth-midiPorts"),
    openPort: (port) => ipcRenderer.invoke("synth-openMidi", port),

    // Synth parameters
    setAmplitude: (value) => ipcRenderer.invoke("synth-setAmplitude", value),
    setOscillatorType: (type, index) =>
        ipcRenderer.invoke("synth-setOscillatorType", type, index),
    setOscillatorAmplitude: (amp, index) =>
        ipcRenderer.invoke("synth-setOscillatorAmplitude", amp, index),

    // ADSR
    setAttack: (v) => ipcRenderer.invoke("synth-setAttack", v),
    setDecay: (v) => ipcRenderer.invoke("synth-setDecay", v),
    setSustain: (v) => ipcRenderer.invoke("synth-setSustain", v),
    setRelease: (v) => ipcRenderer.invoke("synth-setRelease", v),

    // Recording
    startRecording: () => ipcRenderer.invoke("synth-startRecording"),
    stopRecording: () => ipcRenderer.invoke("synth-stopRecording"),
    setRecordingPath: (path) =>
        ipcRenderer.invoke("synth-setRecordingPath", path),

    // Samples
    setSamplesPath: (path) =>
        ipcRenderer.invoke("synth-setSamplesPath", path),
    getOscillatorNames: () =>
        ipcRenderer.invoke("synth-getOscillatorNames"),

    // Waveform preview
    getOscillatorPlot: (name, length) =>
        ipcRenderer.invoke("synth-getOscillatorPlot", name, length),
});
