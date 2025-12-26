const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("synthAPI", {
    // MIDI
    ports: () => ipcRenderer.invoke("synth-midiPorts"),
    openPort: (port) => ipcRenderer.invoke("synth-openMidi", port),

    // Synth parameters
    setAmplitude: (value) => ipcRenderer.invoke("synth-setAmplitude", value),
    setOscillatorType: (type, index) =>
        ipcRenderer.invoke("synth-setOscillatorType", type, index),

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

    // pipelineAPI
    pipelineAddFilter: (filter, idx) => 
        ipcRenderer.invoke("synth-pipelineAddFilter", filter, idx),
    pipelineRemove: (idx) => 
        ipcRenderer.invoke("synth-pipelineRemove", idx),
    pipelineMove: (current, target) => 
        ipcRenderer.invoke("synth-pipelineMove", current, target),
    pipelineSwap: (i1, i2) => 
        ipcRenderer.invoke("synth-pipelineSwap", i1, i2),
    pipelineSetFilterParam: (idx, param, value) => 
        ipcRenderer.invoke("synth-pipelineSetFilterParam", idx, param, value),
    pipelineGetFilterParam: (idx, param) => 
        ipcRenderer.invoke("synth-pipelineGetFilterParam", idx, param),
    pipelineLength: () => 
        ipcRenderer.invoke("synth-pipelineLength"),

    // Samples
    setSamplesPath: (path) =>
        ipcRenderer.invoke("synth-setSamplesPath", path),
    getOscillatorNames: () =>
        ipcRenderer.invoke("synth-getOscillatorNames"),

    // Waveform preview
    getOscillatorPlot: (name, length, step) =>
        ipcRenderer.invoke("synth-getOscillatorPlot", name, length, step),
});

contextBridge.exposeInMainWorld("presetsAPI", {
  read: () => ipcRenderer.invoke("presets:read"),
  write: (data) => ipcRenderer.invoke("presets:write", data),
  saveOne: (name, preset) => ipcRenderer.invoke("presets:saveOne", { name, preset }),
  delete: (name) => ipcRenderer.invoke("presets:delete", name)
});