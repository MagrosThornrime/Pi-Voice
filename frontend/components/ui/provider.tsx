"use client"

import { ChakraProvider, defaultSystem } from "@chakra-ui/react"
import {
  ColorModeProvider,
  type ColorModeProviderProps,
} from "./color-mode"
import { changeParams} from "@/app/sequencer/actions";
import { useEffect } from "react";
import { Preset, PresetFile } from "./presetsProvider";
import { EffectType, FilterType, OptEffectKey, OptKey } from "@/app/utils/tables";

declare global {
    interface Window {
        synthAPI: {
            ports: () => Promise<string[]>;
            openPort: (port: number) => Promise<void>;

            // setters
            setOscillatorType: (type: string, index: number) => Promise<void>;
            setAttack: (v: number) => Promise<void>;
            setDecay: (v: number) => Promise<void>;
            setSustain: (v: number) => Promise<void>;
            setRelease: (v: number) => Promise<void>;
            startRecording: () => Promise<void>;
            stopRecording: () => Promise<void>;

            // pipelineAPI
            pipelineAddFilter: (filter: number, idx: number) => Promise<void>;
            pipelineAddEffect: (effect: number, idx: number) => Promise<void>;
            pipelineRemove: (idx: number) => Promise<void>;
            pipelineMove: (current: number, target: number) => Promise<void>;
            pipelineSwap: (i1: number, i2: number) => Promise<void>;
            pipelineSetFilterParam: (idx: number, param: number, value: number) => Promise<void>;
            pipelineSetEffectParam: (idx: number, param: number, value: number) => Promise<void>;
            pipelineGetFilterParam: (idx: number, param: number) => Promise<number>;
            pipelineGetEffectParam: (idx: number, param: number) => Promise<number>;
            pipelineLength: () => Promise<number>;
            pipelineSetAmplitude: (value: number) => Promise<void>;

            // sequencerAPI
            sequencerIsActive: () => Promise<boolean>;
            sequencerIsRecording: () => Promise<boolean>;
            sequencerActivate: () => Promise<void>;
            sequencerDeactivate: () => Promise<void>;
            sequencerStartRecording: (sampleRate: number, channels: number, seconds: number) => Promise<void>;
            sequencerStopRecording: () => Promise<void>;
            sequencerRemoveSample: (i: number) => Promise<void>;
            sequencerClear: () => Promise<void>;
            sequencerSampleLength: (i: number) => Promise<number>;
            sequencerLength: () => Promise<number>;
            sequencerMoveSample: (curr: number, val: number) => Promise<void>;
            sequencerSwapSamples: (i1: number, i2: number) => Promise<void>;
            sequencerAddSample: (name: string) => Promise<void>;

            // sample manager
            setSamplesPath: (path: string) => Promise<void>;
            getOscillatorNames: () => Promise<string[]>;

            // waveform preview
            getOscillatorPlot: (name: string) => Promise<number[]>;

        };
        presetsAPI: {
            read: () => Promise<PresetFile>;
            write: (data: PresetFile) => Promise<void>;
            saveOne: (name: string, preset: Preset) => Promise<void>;
        };
        slidersAPI: {
            read: (sliderType: "lin" | "log", itemType: "filters" | "effects", sliderVal: number,
                itemName: EffectType | FilterType, paramName:OptEffectKey | OptKey, change:boolean, bounds?: number[]) => Promise<changeParams | number>;
            write: () => Promise<void>;
        };
    }
}

export function Provider(props: ColorModeProviderProps) {
  useEffect(() => {
        window.slidersAPI.write();
    }, []);
  return (
    <ChakraProvider value={defaultSystem}>
      <ColorModeProvider {...props} />
    </ChakraProvider>
  )
}
