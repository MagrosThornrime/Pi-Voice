export type Opt = {
    continuous?: boolean;
    logScale?: boolean;
    range: number[];
    step?: number; // log scale will not require step, neither continuous parameters
    index: number;
}

export type FilterType = "allpass" | "bandpass" | "highpass" | "lowpass" | "highshelf" | "lowshelf" | "notch" | "peakingeq";

export type EffectType = "chorus" | "delay" | "reverb" | "robotify" | "phaser";
//"noiseReduction" | "equalizer" | "fades" | "echo" | "glitches" 

export type Filter = {
    label: string;
    value: string;
    opts: Record<OptKey, Opt>;
};

export type EffectOptionsMap = {
    chorus: "baseDelayFactor" | "modFrequency" | "modDepth" | "bufferFrames" | "feedback" | "mix",
    delay: "bufferFrames" | "feedback" | "mix",
    reverb: "bufferFrames" | "feedback" | "mix",
    robotify: "modFrequency",
    phaser: "minHz" | "rangeHz" | "rateHz" | "feedback" | "mix",
};

export type Effect<T extends EffectType = EffectType> = {
    label: string;
    value: T;
    //opts: Record<EffectOptionsMap[T], Opt>;
}

export type OptKey = "order" | "cutoff" | "gainDB" | "quality";

export type OptEffectKey = "baseDelayFactor" | "modFrequency" | "modDepth" | "bufferFrames" | "feedback" | "mix";

export const defaultOpts: Record<OptKey, Opt> = {
    order: { continuous: false, logScale: false, range: [0, 1], step: 1, index: 3 },
    cutoff: { continuous: false, logScale: true, range: [10, 20000], index: 0 },
    gainDB: { continuous: true, logScale: false, range: [-24, 24], index: 2 },
    quality: { continuous: true, logScale: false, range: [0.1, 20.0], index: 1 }
}


function getLinParams(range:number[], idx:number, step?: number):Opt{
    if (step != undefined){
        return { continuous: true, logScale: false, range: range, index: idx, step: step};
    }

    return { continuous: true, logScale: false, range: range, index: idx };
}

function getLogParams( range: number[], idx: number, step?: number): Opt {
  if (step !== undefined) {
    return { continuous: true, logScale: true, range: range, index: idx, step: step };
  }

  return { continuous: true, logScale: true, range: range, index: idx };
}


export const defaultEffectOpts: Record<EffectType, Record<string, Opt>> = {
    chorus: {
        baseDelayFactor: getLinParams([0, 0.1], 0),
        modFrequency: getLinParams([0.5, 2], 1),
        modDepth: getLinParams([0, 0.005], 2),
        bufferFrames: getLogParams([500, 20000], 3),
        feedback: getLinParams([0, 1], 4),
        mix: getLinParams([0, 1], 5),
    },
    delay: {
        bufferFrames: getLogParams([500, 20000], 0),
        feedback: getLinParams([0, 1], 1),
        mix: getLinParams([0, 1], 2),
    },
    reverb: {
        bufferFrames: getLogParams([500, 20000], 0),
        feedback: getLinParams([0, 1], 1),
        mix: getLinParams([0, 1], 2),
    },
    robotify: {
        modFrequency: getLinParams([1, 50], 0),
    },
    phaser: {
        minHz:    getLogParams([50, 1000], 0),
        rangeHz:  getLogParams([100, 3000], 1),
        rateHz:   getLinParams([0.05, 5], 2),
        feedback: getLinParams([0, 1], 3),
        mix:      getLinParams([0, 1], 4),
    },
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
    { label: "Chorus", value: "chorus" },
    { label: "Delay", value: "delay" },
    { label: "Reverb", value: "reverb" },
    { label: "Robotify", value: "robotify" },
    { label: "Phaser", value: "phaser" },
]
    // { label: "equalizer", value: "equalizer" },
    // { label: "fades", value: "fades" },
    // { label: "noise reduction", value: "noiseReduction" }
    // { label: "glitches", value: "glitches" },
    // { label: "echo", value: "echo" },

export const effectIDs:Record<EffectType, number> = {
    chorus: 0,
    delay: 1,
    reverb: 2,
    robotify: 3,
    phaser: 4,
}