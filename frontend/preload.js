const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("synthAPI", {
    // MIDI
    ports: () => ipcRenderer.invoke("synth-ports"),
    openPort: (port) => ipcRenderer.invoke("synth-open", port),
    // Parameter controls
    setAmplitude: (value) => ipcRenderer.invoke("synth-setAmplitude", value),
    setOscillatorType: (type, index) => ipcRenderer.invoke("synth-setOscillatorType", type, index),
    setOscillatorAmplitude: (amp, index) => ipcRenderer.invoke("synth-setOscillatorAmplitude", amp, index),
    setAttack: (value) => ipcRenderer.invoke("synth-setAttack", value),
    setDecay: (value) => ipcRenderer.invoke("synth-setDecay", value),
    setSustain: (value) => ipcRenderer.invoke("synth-setSustain", value),
    setRelease: (value) => ipcRenderer.invoke("synth-setRelease", value),

    // Filters
    addFilter: (name) => ipcRenderer.invoke("synth-addFilter", name),
    clearFilters: () => ipcRenderer.invoke("synth-clearFilters"),
});

contextBridge.exposeInMainWorld("presetsAPI", {
  read: () => ipcRenderer.invoke("presets:read"),
  write: (data) => ipcRenderer.invoke("presets:write", data),
  saveOne: (name, preset) => ipcRenderer.invoke("presets:saveOne", { name, preset }),
  delete: (name) => ipcRenderer.invoke("presets:delete", name)
});