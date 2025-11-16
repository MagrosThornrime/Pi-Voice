const path = require("path");
const { app, BrowserWindow, ipcMain } = require("electron");

let synth;
try {
  synth = require(path.join(__dirname, "lib", "BackendApp.node")); // updated name
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

// =========================
// Synth parameter controls
// =========================
ipcMain.handle("synth-setAmplitude", (e, value) => synth.setAmplitude(value));
ipcMain.handle("synth-setOscillatorType", (e, type, index) => synth.setOscillatorType(type, index));
ipcMain.handle("synth-setOscillatorAmplitude", (e, amp, index) => synth.setOscillatorAmplitude(amp, index));
ipcMain.handle("synth-setAttack", (e, val) => synth.setAttack(val));
ipcMain.handle("synth-setDecay", (e, val) => synth.setDecay(val));
ipcMain.handle("synth-setSustain", (e, val) => synth.setSustain(val));
ipcMain.handle("synth-setRelease", (e, val) => synth.setRelease(val));
ipcMain.handle("synth-startRecording", (e) => synth.startRecording());
ipcMain.handle("synth-stopRecording", (e) => synth.stopRecording());
ipcMain.handle("synth-setRecordingPath", (e, val) => synth.setRecordingPath(val));