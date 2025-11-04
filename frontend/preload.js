const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("synthAPI", {
    ports: () => ipcRenderer.invoke("synth-ports"),
    openPort: (port) => ipcRenderer.invoke("synth-open", port),
    start: () => ipcRenderer.invoke("synth-start"),
    stop: () => ipcRenderer.invoke("synth-stop"),
    cleanup: () => ipcRenderer.invoke("synth-cleanup"),
});
