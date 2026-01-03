export type Opt = {
    continuous?: boolean;
    logScale?: boolean;
    range: number[];
    step?: number; // log scale will not require step, neither continuous parameters
    index: number;
}

export type FilterType = "allpass" | "bandpass" | "highpass" | "lowpass" | "highshelf" | "lowshelf" | "notch" | "peakingeq";

export type EffectType = "chorus" | "delay" | "reverb" | "robotify";
//"noiseReduction" | "equalizer" | "fades" | "echo" | "glitches" 

export type Filter = {
    label: string;
    value: string;
    opts: Record<OptKey, Opt>;
};

export type EffectOptionsMap = {
    chorus: "baseDelayFactor" | "modFrequency" | "modDepth" | "bufferFrames" | "feedback" | "wetAmount" | "count",
    delay: "bufferFrames" | "feedback" | "wetAmount" | "count",
    reverb: "bufferFrames" | "feedback" | "wetAmount" | "count",
    robotify: "modFrequency" | "count",
};

export type Effect<T extends EffectType = EffectType> = {
    label: string;
    value: T;
    //opts: Record<EffectOptionsMap[T], Opt>;
}

export type OptKey = "order" | "cutoff" | "gainDB" | "quality";

export type OptEffectKey = "baseDelayFactor" | "modFrequency" | "modDepth" | "bufferFrames" | "feedback" | "wetAmount" | "count";

export const defaultOpts: Record<OptKey, Opt> = {
    order: { continuous: false, logScale: false, range: [0, 1], step: 1, index: 3 },
    cutoff: { continuous: false, logScale: true, range: [10, 20000], index: 0 },
    gainDB: { continuous: true, logScale: false, range: [-24, 24], index: 2 },
    quality: { continuous: true, logScale: false, range: [0.1, 20.0], index: 1 }
}

export const exampleParamsLin = { continuous: true, logScale: false, range: [-24, 24], index: 1 }
export const exampleParamsLog = { continuous: true, logScale: false, range: [100, 50000], index: 2 }


export const defaultEffectOpts: Record<EffectType, Record<string, Opt>> = {
    chorus: {
        baseDelayFactor: exampleParamsLin,
        modFrequency: exampleParamsLog,
        modDepth: exampleParamsLin,
        bufferFrames: exampleParamsLin,
        feedback: exampleParamsLog,
        wetAmount: exampleParamsLin,
        count: exampleParamsLin
    },
    delay: {
        bufferFrames: exampleParamsLin,
        feedback: exampleParamsLog,
        wetAmount: exampleParamsLin,
        count: exampleParamsLin
    },
    reverb: {
        bufferFrames: exampleParamsLin,
        feedback: exampleParamsLog,
        wetAmount: exampleParamsLin,
        count: exampleParamsLin
    },
    robotify: {
        modFrequency: exampleParamsLog,
        count: exampleParamsLin
    }
}


export const filters: Filter[] = [
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


export const effects: Effect[] = [
    { label: "chorus", value: "chorus" },
    { label: "delay", value: "delay" },
    { label: "reverb", value: "reverb" },
    { label: "robotify", value: "robotify" }
]
    // { label: "equalizer", value: "equalizer" },
    // { label: "fades", value: "fades" },
    // { label: "noise reduction", value: "noiseReduction" }
    // { label: "glitches", value: "glitches" },
    // { label: "echo", value: "echo" },

export const effectIDs:Record<EffectType, number> = {
    chorus: 0,
    reverb: 1,
    delay: 2,
    robotify: 3
}