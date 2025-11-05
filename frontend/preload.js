const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("synthAPI", {
    // MIDI
    ports: () => ipcRenderer.invoke("synth-ports"),
    openPort: (port) => ipcRenderer.invoke("synth-open", port),

    // Synth control
    start: () => ipcRenderer.invoke("synth-start"),
    stop: () => ipcRenderer.invoke("synth-stop"),
    cleanup: () => ipcRenderer.invoke("synth-cleanup"),

    // Parameter controls
    setAmplitude: (value) => ipcRenderer.invoke("synth-setAmplitude", value),
    setOscillatorType: (index, type) => ipcRenderer.invoke("synth-setOscillatorType", index, type),
    setOscillatorAmplitude: (index, amp) => ipcRenderer.invoke("synth-setOscillatorAmplitude", index, amp),
    setAttack: (value) => ipcRenderer.invoke("synth-setAttack", value),
    setDecay: (value) => ipcRenderer.invoke("synth-setDecay", value),
    setSustain: (value) => ipcRenderer.invoke("synth-setSustain", value),
    setRelease: (value) => ipcRenderer.invoke("synth-setRelease", value),

    // Filters
    addFilter: (name) => ipcRenderer.invoke("synth-addFilter", name),
    clearFilters: () => ipcRenderer.invoke("synth-clearFilters"),
});
