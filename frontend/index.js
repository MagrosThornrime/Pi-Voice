const path = require("path");
const { app, BrowserWindow, ipcMain } = require("electron");
const { spawn, exec } = require("child_process");

let nextProcess;
let synth;

// Load native module
try {
  synth = require(path.join(__dirname, "lib", "BackendApp.node"));
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

app.whenReady().then(() => {
  nextProcess = spawn("npm", ["run", "dev"], {
    cwd: path.join(process.cwd(), "./frontend"),
    shell: true,
    stdio: "inherit",
  });

  setTimeout(createWindow, 100);
});

// Kill frontend dev server on exit
app.on("quit", () => {
  if (nextProcess) {
    if (process.platform === "win32") {
      exec(`taskkill /PID ${nextProcess.pid} /T /F`);
    } else {
      nextProcess.kill();
    }
  }
});


// ===================================================
// IPC Mappings — EXACTLY matching your C++ module
// ===================================================

// MIDI
ipcMain.handle("synth-midiPorts", () => synth.midiPorts());
ipcMain.handle("synth-openMidi", (e, port) => synth.openMidi(port));

// Synth parameters
ipcMain.handle("synth-setAmplitude", (e, v) => synth.setAmplitude(v));
ipcMain.handle("synth-setOscillatorType", (e, type, index) =>
    synth.setOscillatorType(type, index)
);
ipcMain.handle("synth-setOscillatorAmplitude", (e, amp, index) =>
    synth.setOscillatorAmplitude(amp, index)
);

// ADSR
ipcMain.handle("synth-setAttack", (e, v) => synth.setAttack(v));
ipcMain.handle("synth-setDecay", (e, v) => synth.setDecay(v));
ipcMain.handle("synth-setSustain", (e, v) => synth.setSustain(v));
ipcMain.handle("synth-setRelease", (e, v) => synth.setRelease(v));

// Recording
ipcMain.handle("synth-startRecording", () => synth.startRecording());
ipcMain.handle("synth-stopRecording", () => synth.stopRecording());
ipcMain.handle("synth-setRecordingPath", (e, path) =>
    synth.setRecordingPath(path)
);

// Samples
ipcMain.handle("synth-setSamplesPath", (e, path) =>
    synth.setSamplesPath(path)
);
ipcMain.handle("synth-getOscillatorNames", () =>
    synth.getOscillatorsNames()
);

// Waveforms
ipcMain.handle("synth-getOscillatorPlot", (e, name, len) =>
    synth.getOscillatorPlot(name, len)
);
