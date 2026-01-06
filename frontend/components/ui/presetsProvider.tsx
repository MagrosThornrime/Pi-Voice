"use client";

import { ItemsParams } from "@/app/utils/context_utils";
import { createContext, useContext, useState, useEffect, ReactNode, Dispatch, SetStateAction, useRef } from "react";
import {getADSRFactor} from "@/app/utils/maths_utils";
import {adsrRanges} from "@/app/utils/tables";

export interface Preset {
  attack : number;
  decay : number;
  sustain : number;
  release : number;
  volume : number;
  oscilator1 : string;
  oscilator2 : string;
  oscilator3 : string;
  filters : ItemsParams[];
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
  loadPreset: (name: string) => void;
}

const PresetContext = createContext<PresetContextType | undefined>(undefined);

export function PresetProvider({ children }: { children: ReactNode }) {
  const isFirstRender = useRef(0);
  const [presetNr, setPresetNr] = useState(1);
  const maxPresets = 5;

  const [presetProperties, setPresetProperties] = useState<Preset>({
    attack:10,
    decay:20,
    sustain:20,
    release:40,
    volume:10,
    oscilator1:"sine",
    oscilator2:"empty",
    oscilator3:"empty",
    filters:[],
  })

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
    let preset: Preset = {
      ...presetProperties,
      attack: presetProperties.attack / 100,
      decay: presetProperties.decay / 100,
      sustain: presetProperties.sustain / 100,
      release: presetProperties.release / 100,
      volume: presetProperties.volume / 100,
    };
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
        filters:preset.filters,
      });
      window.synthAPI.setAttack(getADSRFactor(preset.attack, adsrRanges.attack.min, adsrRanges.attack.max));
      window.synthAPI.setDecay(getADSRFactor(preset.decay, adsrRanges.decay.min, adsrRanges.decay.max));
      window.synthAPI.setSustain(preset.sustain);
      window.synthAPI.setRelease(getADSRFactor(preset.release, adsrRanges.release.min, adsrRanges.release.max));
      window.synthAPI.setAmplitude(preset.volume);
      window.synthAPI.setOscillatorType(preset.oscilator1,0);
      window.synthAPI.setOscillatorType(preset.oscilator1,1);
      window.synthAPI.setOscillatorType(preset.oscilator1,2);
    });
  }

  useEffect(() => {
    if (isFirstRender.current<2){
      isFirstRender.current = isFirstRender.current+1;
    }else{
      savePreset(String(presetNr));
      console.log("preset saved");
    }
  }, [presetProperties]);

  return (
    <PresetContext.Provider
      value={{
        presetNr,
        setPresetNr,
        maxPresets,
        presetProperties,
        setPresetProperties,
        loadPreset,
      }}
    >
      {children}
    </PresetContext.Provider>
  );
}

export function usePreset() {
  const ctx = useContext(PresetContext);
  if (!ctx) throw new Error("usePreset must be used inside PresetProvider");
  return ctx;
}
