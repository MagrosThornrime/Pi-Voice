const path = require("path");
const { app, BrowserWindow, ipcMain } = require("electron");
const { spawn } = require("child_process");
const { exec } = require("child_process");
const fs = require("fs");

const presetFile = path.join(app.getPath("userData"), "presets.json");

function ensurePresetFile() {
  if (!fs.existsSync(presetFile)) {
    fs.writeFileSync(
      presetFile,
      JSON.stringify({ presets: {}, lastUsed: null }, null, 2)
    );
  }
}

ensurePresetFile();

ipcMain.handle("presets:read", () => {
  ensurePresetFile();
  const raw = fs.readFileSync(presetFile, "utf-8");
  return JSON.parse(raw);
});

ipcMain.handle("presets:write", (event, data) => {
  fs.writeFileSync(presetFile, JSON.stringify(data, null, 2));
  return true;
});

ipcMain.handle("presets:saveOne", (event, { name, preset }) => {
  const data = JSON.parse(fs.readFileSync(presetFile, "utf-8"));
  data.presets[name] = preset;
  data.lastUsed = name;
  fs.writeFileSync(presetFile, JSON.stringify(data, null, 2));
  return true;
});

ipcMain.handle("presets:delete", (event, name) => {
  const data = JSON.parse(fs.readFileSync(presetFile, "utf-8"));
  delete data.presets[name];

  if (data.lastUsed === name) {
    data.lastUsed = null;
  }

  fs.writeFileSync(presetFile, JSON.stringify(data, null, 2));
  return true;
});

let nextProcess;
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

app.whenReady().then(async() => {
  nextProcess = spawn("npm", ["run", "dev"], {
    cwd: path.join(process.cwd(), "./frontend"),
    shell: true,
    stdio: "inherit",
  });

  try {
    setTimeout(() => {
      createWindow();
    }, 100);
  } catch (err) {
    console.error(err);
    app.quit();
  }
});

app.on("quit", () => {
  if (nextProcess) {
    if (process.platform === "win32") {
      exec(`taskkill /PID ${nextProcess.pid} /T /F`);
    } else {
      nextProcess.kill();
    }
  }
});

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
