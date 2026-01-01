export type Opt = {
    mutable: boolean;
    continuous?: boolean;
    logScale?: boolean;
    range: number[];
    step?: number; // log scale will not require step, neither continuous parameters
    index: number;
}

export type Filter = {
    label: string;
    value: string;
    opts: Record<string, Opt>;
};

export type OptKey = "order" | "cutoff" | "gainDB" | "quality";

export type OptEffectKey = "param" | "param1" | "param2" | "param3";

export const defaultOpts: Record<OptKey, Opt> = {
    order: { mutable: true, continuous: false, logScale: false, range: [0, 1], step: 1, index: 3 } ,
    cutoff: { mutable: true, continuous: false, logScale: true, range: [10, 20000], index: 0 } ,
    gainDB: {mutable: true, continuous: true, logScale: false, range: [-24, 24], index: 2 },
    quality: {mutable: true, continuous: true, logScale: false, range: [0.1, 20.0], index: 1 }
}

export const defaultEffectOpts: Record <OptEffectKey, Opt> = {
    param: { mutable: true, continuous: false, logScale: false, range: [0, 1], step: 1, index: 3 } ,
    param1: { mutable: true, continuous: false, logScale: true, range: [10, 20000], index: 0 } ,
    param2: {mutable: true, continuous: true, logScale: false, range: [-24, 24], index: 2 },
    param3: {mutable: true, continuous: true, logScale: false, range: [0.1, 20.0], index: 1 }
}

export type FilterType = "allpass" | "bandpass" | "highpass" | "lowpass" | "highshelf" | "lowshelf" | "notch" | "peakingeq";

export const filters:Filter[] = [
    { 
        label: "AllPass", 
        value: "allpass", 
        opts: defaultOpts
    },
    { 
        label: "BandPass",
        value: "bandpass",
        opts: defaultOpts
    },
    { 
        label: "HighPass",
        value: "highpass",
        opts: defaultOpts
    },
    { 
        label: "LowPass",
        value: "lowpass",
        opts: defaultOpts 
    },
    {
        label: "HighShelf",
        value: "highshelf",
        opts: defaultOpts
    },
    { 
        label: "LowShelf",
        value: "lowshelf",
        opts: defaultOpts 
    },
    { 
        label: "Notch",
        value: "notch",
        opts: defaultOpts 
    },
    {
        label: "PeakingEQ",
        value: "peakingeq",
        opts: defaultOpts
    },
]


export const effects:Filter[] = [
    { label: "echo", value: "echo", opts: defaultEffectOpts },
    { label: "chorus", value: "chorus", opts: defaultEffectOpts},
    { label: "delay", value: "delay", opts: defaultEffectOpts },
    { label: "glitches", value: "glitches", opts: defaultEffectOpts },
    { label: "reverb", value: "reverb", opts: defaultEffectOpts},
    { label: "equalizer", value: "equalizer", opts: defaultEffectOpts},
    { label: "fades", value: "fades", opts: defaultEffectOpts },
    { label: "noise reduction", value: "noise reduction", opts: defaultEffectOpts}
]