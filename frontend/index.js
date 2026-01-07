const path = require("path");
const { app, BrowserWindow, ipcMain } = require("electron");
const { spawn, exec } = require("child_process");
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
  synth = require(path.join(__dirname, "lib", "BackendApp.node"));
  console.log("Loaded synth addon");
} catch (err) {
  console.error("Failed to load synth addon:", err);
  app.quit();
}

function createWindow() {
  const win = new BrowserWindow({
    fullscreen: true,
    width : 1024,
    height: 600,
    fullscreenable: true,
    autoHideMenuBar: true,
    webPreferences: {
      preload: path.join(__dirname, "preload.js"),
      contextIsolation: true,
    },
  });

  win.loadURL("http://localhost:3000");
}

app.whenReady().then(async () => {
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

// MIDI
ipcMain.handle("synth-midiPorts", () => synth.midiPorts());
ipcMain.handle("synth-openMidi", (e, port) => synth.openMidi(port));

// pipelineAPI
ipcMain.handle("synth-pipelineAddFilter", (e, filter, idx) =>
    synth.pipelineAddFilter(filter, idx)
);
ipcMain.handle("synth-pipelineAddEffect", (e, effect, idx) =>
    synth.pipelineAddEffect(effect, idx)
);
ipcMain.handle("synth-pipelineRemove", (e, idx) =>
    synth.pipelineRemove(idx)
);
ipcMain.handle("synth-pipelineMove", (e, current, target) =>
    synth.pipelineMove(current, target)
);
ipcMain.handle("synth-pipelineSwap", (e, i1, i2) =>
    synth.pipelineSwap(i1, i2)
);
ipcMain.handle("synth-pipelineSetFilterParam", (e, idx, param, value) =>
    synth.pipelineSetFilterParam(idx, param, value)
);
ipcMain.handle("synth-pipelineGetFilterParam", (e, idx, param) =>
    synth.pipelineGetFilterParam(idx, param)
);
ipcMain.handle("synth-pipelineSetEffectParam", (e, idx, param, value) =>
    synth.pipelineSetEffectParam(idx, param, value)
);
ipcMain.handle("synth-pipelineGetEffectParam", (e, idx, param) =>
    synth.pipelineGetEffectParam(idx, param)
);
ipcMain.handle("synth-pipelineLength", () =>
    synth.pipelineLength()
);

// sequencerAPI
ipcMain.handle("synth-sequencerIsActive", () =>
    synth.sequencerIsActive()
);
ipcMain.handle("synth-sequencerIsRecording", () =>
    synth.sequencerIsRecording()
);
ipcMain.handle("synth-sequencerActivate", () =>
    synth.sequencerActivate()
);
ipcMain.handle("synth-sequencerDeactivate", () =>
    synth.sequencerDeactivate()
);
ipcMain.handle(
    "synth-sequencerStartRecording",
    (e, sampleRate, channels, seconds) =>
        synth.sequencerStartRecording(sampleRate, channels, seconds)
);
ipcMain.handle("synth-sequencerStopRecording", () =>
    synth.sequencerStopRecording()
);
ipcMain.handle("synth-sequencerRemoveSample", (e, i) =>
    synth.sequencerRemoveSample(i)
);
ipcMain.handle("synth-sequencerClear", () =>
    synth.sequencerClear()
);
ipcMain.handle("synth-sequencerSampleLength", (e, i) =>
    synth.sequencerLength(i)
);
ipcMain.handle("synth-sequencerLength", () =>
    synth.sequencerLength()
);
ipcMain.handle("synth-sequencerMoveSample", (e, curr, target) =>
    synth.sequencerMoveSample(curr, target)
);
ipcMain.handle("synth-sequencerSwapSamples", (e, i1, i2) =>
    synth.sequencerSwapSamples(i1, i2)
);
ipcMain.handle("synth-sequencerAddSample", (e, name) =>
    synth.sequencerAddSample(name)
);

// Synth parameters
ipcMain.handle("synth-setAmplitude", (e, v) =>
    synth.setAmplitude(v)
);
ipcMain.handle("synth-setOscillatorType", (e, type, index) =>
    synth.setOscillatorType(type, index)
);

// ADSR
ipcMain.handle("synth-setAttack", (e, v) =>
    synth.setAttack(v)
);
ipcMain.handle("synth-setDecay", (e, v) =>
    synth.setDecay(v)
);
ipcMain.handle("synth-setSustain", (e, v) =>
    synth.setSustain(v)
);
ipcMain.handle("synth-setRelease", (e, v) =>
    synth.setRelease(v)
);

// Recording
ipcMain.handle("synth-startRecording", () =>
    synth.startRecording()
);
ipcMain.handle("synth-stopRecording", () =>
    synth.stopRecording()
);
ipcMain.handle("synth-setRecordingPath", (e, path) =>
    synth.setRecordingPath(path)
);

// Samples
ipcMain.handle("synth-setSamplesPath", (e, path) =>
    synth.setSamplesPath(path)
);
ipcMain.handle("synth-getOscillatorNames", () =>
    synth.getOscillatorNames()
);

// Waveforms
ipcMain.handle("synth-getOscillatorPlot", (e, name, len, step) =>
    synth.getOscillatorPlot(name, len, step)
);
