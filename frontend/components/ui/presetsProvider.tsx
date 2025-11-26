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
      const last = Number(data.lastUsed) ?? 1;
      const preset = data.presets[String(last)];
      if (!preset) return;
      loadPreset(String(last));
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
      window.synthAPI.setAttack(10 ** (-preset.attack * 10));
      setDecayValue([preset.decay * 100]);
      window.synthAPI.setDecay(10 ** (-preset.decay * 10));
      setSustainValue([preset.sustain * 100]);
      window.synthAPI.setSustain(preset.sustain);
      setReleaseValue([preset.release * 100]);
      window.synthAPI.setRelease(10 ** (-preset.release * 10));
      setVolumeValue([preset.volume * 100]);
      window.synthAPI.setAmplitude(preset.volume);
      setOscillator1(preset.oscilator1);
      window.synthAPI.setOscillatorType(preset.oscilator1,0);
      setOscillator2(preset.oscilator2);
      window.synthAPI.setOscillatorType(preset.oscilator1,1);
      setOscillator3(preset.oscilator3);
      window.synthAPI.setOscillatorType(preset.oscilator1,2);
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
