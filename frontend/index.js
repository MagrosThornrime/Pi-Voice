const path = require("path");
const { app, BrowserWindow, ipcMain } = require("electron");

let synth;
try {
  synth = require(path.join(__dirname, "lib", "SimpleSynthExample.node")); // updated name
  console.log("Loaded synth addon");
} catch (err) {
  console.error("Failed to load synth addon:", err);
  app.quit();
}

function createWindow() {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, "preload.js"),
      contextIsolation: true,
    },
  });

  win.loadURL("http://localhost:3000");
}

app.whenReady().then(createWindow);

// =========================
// IPC handlers
// =========================

// MIDI
ipcMain.handle("synth-ports", () => synth.midiPorts());
ipcMain.handle("synth-open", (event, port) => synth.openMidi(port));

// Synth lifecycle
ipcMain.handle("synth-start", () => synth.startSynth());
ipcMain.handle("synth-stop", () => synth.stopSynth());
ipcMain.handle("synth-cleanup", () => synth.cleanup());

// =========================
// Synth parameter controls
// =========================
ipcMain.handle("synth-setAmplitude", (e, value) => synth.setAmplitude(value));
ipcMain.handle("synth-setOscillatorType", (e, index, type) => synth.setOscillatorType(index, type));
ipcMain.handle("synth-setOscillatorAmplitude", (e, index, amp) => synth.setOscillatorAmplitude(index, amp));
ipcMain.handle("synth-setAttack", (e, val) => synth.setAttack(val));
ipcMain.handle("synth-setDecay", (e, val) => synth.setDecay(val));
ipcMain.handle("synth-setSustain", (e, val) => synth.setSustain(val));
ipcMain.handle("synth-setRelease", (e, val) => synth.setRelease(val));

// Filters
ipcMain.handle("synth-addFilter", (e, name) => synth.addFilter(name));
ipcMain.handle("synth-clearFilters", () => synth.clearFilters());
