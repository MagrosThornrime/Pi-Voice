"use client";

import { createContext, useContext, useState, useEffect } from "react";

const PresetContext = createContext(null);

export function PresetProvider({ children }) {
  const [presetNr, setPresetNr] = useState(1);
  const maxPresets = 5;

  const [volumeValue, setVolumeValue] = useState([10]);
  const [attackValue, setAttackValue] = useState([10]);
  const [decayValue, setDecayValue] = useState([20]);
  const [sustainValue, setSustainValue] = useState([20]);
  const [releaseValue, setReleaseValue] = useState([40]);
  const [oscilator1, setOscillator1] = useState("sine")
  const [oscilator2, setOscillator2] = useState("empty")
  const [oscilator3, setOscillator3] = useState("empty")

  useEffect(() => {
    window.presetsAPI.read().then((data) => {
      const last = data.lastUsed ?? "1";
      const preset = data.presets[last];
      if (!preset) return;

      setAttackValue([preset.attack * 100]);
      setDecayValue([preset.decay * 100]);
      setSustainValue([preset.sustain * 100]);
      setReleaseValue([preset.release * 100]);
      setVolumeValue([preset.volume * 100]);
      setPresetNr(last);
    });
  }, []);

  function savePreset(name) {
    const preset = {
      attack: attackValue[0] / 100,
      decay: decayValue[0] / 100,
      sustain: sustainValue[0] / 100,
      release: releaseValue[0] / 100,
      volume: volumeValue[0] / 100,
      oscilator1: oscilator1,
      oscilator2: oscilator2,
      oscilator3: oscilator3,
    };

    window.presetsAPI.saveOne(name, preset);
  }

  function loadPreset(name) {
    window.presetsAPI.read().then((data) => {
      const preset = data.presets[name];
      if (!preset) return;

      setAttackValue([preset.attack * 100]);
      setDecayValue([preset.decay * 100]);
      setSustainValue([preset.sustain * 100]);
      setReleaseValue([preset.release * 100]);
      setVolumeValue([preset.volume * 100]);
      setOscillator1(preset.oscilator1);
      setOscillator2(preset.oscilator2);
      setOscillator3(preset.oscilator3);
    });
  }

  return (
    <PresetContext.Provider
      value={{
        presetNr,
        setPresetNr,
        maxPresets,
        attackValue,
        setAttackValue,
        decayValue,
        setDecayValue,
        sustainValue,
        setSustainValue,
        releaseValue,
        setReleaseValue,
        volumeValue,
        setVolumeValue,
        oscilator1,
        setOscillator1,
        oscilator2,
        setOscillator2,
        oscilator3,
        setOscillator3,
        savePreset,
        loadPreset,
      }}
    >
      {children}
    </PresetContext.Provider>
  );
}

export function usePreset() {
  return useContext(PresetContext);
}
