"use client";

import { createContext, useContext, useState, useEffect, ReactNode, Dispatch, SetStateAction } from "react";

export interface Preset {
  attack : number;
  decay : number;
  sustain : number;
  release : number;
  volume : number;
  oscilator1 : string;
  oscilator2 : string;
  oscilator3 : string;
}

export interface PresetFile {
  presets: Record<string, Preset>;
  lastUsed : string;
}

interface PresetContextType {
  presetNr: number;
  setPresetNr: Dispatch<SetStateAction<number>>;
  maxPresets: number;
  presetProperties: Preset;
  setPresetProperties: Dispatch<SetStateAction<Preset>>;
  savePreset: (name: string) => void;
  loadPreset: (name: string) => void;
}

const PresetContext = createContext<PresetContextType | undefined>(undefined);

export function PresetProvider({ children }: { children: ReactNode }) {
  const [presetNr, setPresetNr] = useState(1);
  const maxPresets = 5;

  const [presetProperties, setPresetProperties] = useState<Preset>({attack:10,decay:20,sustain:20,release:40,volume:10,oscilator1:"sine",oscilator2:"empty",oscilator3:"empty"})

  useEffect(() => {
    window.presetsAPI.read().then((data) => {
      const last = Number(data.lastUsed) ?? 1;
      const preset = data.presets[last];
      if (!preset) return;
      loadPreset(String(last));
      setPresetNr(last);
    });
  }, []);

  function savePreset(name:string) {
    const preset : Preset = presetProperties;

    window.presetsAPI.saveOne(name, preset);
  }

  function loadPreset(name:string) {
    window.presetsAPI.read().then((data) => {
      const preset = data.presets[Number(name)];
      if (!preset) return;
      
      setPresetProperties({
        attack:preset.attack * 100,
        decay:preset.decay * 100,
        sustain:preset.sustain * 100,
        release:preset.release * 100,
        volume:preset.volume * 100,
        oscilator1:preset.oscilator1,
        oscilator2:preset.oscilator2,
        oscilator3:preset.oscilator3,
      });
      window.synthAPI.setAttack(10 ** (-preset.attack * 10));
      window.synthAPI.setDecay(10 ** (-preset.decay * 10));
      window.synthAPI.setSustain(preset.sustain);
      window.synthAPI.setRelease(10 ** (-preset.release * 10));
      window.synthAPI.setAmplitude(preset.volume);
      window.synthAPI.setOscillatorType(preset.oscilator1,0);
      window.synthAPI.setOscillatorType(preset.oscilator1,1);
      window.synthAPI.setOscillatorType(preset.oscilator1,2);
    });
  }

  return (
    <PresetContext.Provider
      value={{
        presetNr,
        setPresetNr,
        maxPresets,
        presetProperties,
        setPresetProperties,
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
