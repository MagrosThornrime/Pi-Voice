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

    startRecording: () => ipcRenderer.invoke("synth-startRecording"),
    stopRecording: () => ipcRenderer.invoke("synth-stopRecording"),
    setRecordingPath: (value) => ipcRenderer.invoke("synth-setRecordingPath", value),

    
    pipelineAddFilter: (filter, idx) => ipcRenderer.invoke("synth-pipelineAddFilter", filter, idx),
    pipelineRemove: (idx) => ipcRenderer.invoke("synth-pipelineRemove", idx),
    pipelineMove: (current, target) => ipcRenderer.invoke("synth-pipelineMove", current, target),
    pipelineSwap: (i1, i2) => ipcRenderer.invoke("synth-pipelineSwap", i1, i2),
    pipelineSetFilterParam: (idx, param, value) => ipcRenderer.invoke("synth-pipelineSetFilterParam", idx, param, value),
    pipelineGetFilterParam: (idx, param) => ipcRenderer.invoke("synth-pipelineGetFilterParam", idx, param),
    pipelineLength: () => ipcRenderer.invoke("synth-pipelineLength"),
});
