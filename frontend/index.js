const path = require("path");
const { app, BrowserWindow, ipcMain } = require("electron");

let synth;
try {
  synth = require(path.join(__dirname, "lib", "SimpleSynthExample.node"));
  console.log("✅ Loaded synth addon");
} catch (err) {
  console.error("❌ Failed to load synth addon:", err);
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

// Synth control
ipcMain.handle("synth-start", () => synth.startSynth());
ipcMain.handle("synth-stop", () => synth.stopSynth());
ipcMain.handle("synth-cleanup", () => synth.cleanup());
