import { filters, EffectID, EffectType, FilterType } from "./tables";
import { Point } from "../oscilators/page";

export async function clearFilters(filtersNumber: number) {
    for (let i = filtersNumber - 1; i >= 0; i--) {
        await window.synthAPI.pipelineRemove(i);
    }
}

export async function addFilters(filtersArr: string[]) {
    for (let [actIndex, actFilter] of filtersArr.entries()) {
        const filterNumber = filters.findIndex(i => i.value === actFilter);
        await window.synthAPI.pipelineAddFilter(filterNumber, actIndex);
    }
}

export async function addFilter(item: FilterType, idx: number) {
    const filterNumber = filters.findIndex(i => i.value === item);
    await window.synthAPI.pipelineAddFilter(filterNumber, idx);
}

export async function addEffect(item: EffectType, idx: number){
    const effectNumber = EffectID[item]; 
    await window.synthAPI.pipelineAddEffect(effectNumber, idx);
}

export async function setFilterParam(item: FilterType, idx:number, param: number, value: number) {
    console.log("FILTER IDX", idx);
    await window.synthAPI.pipelineSetFilterParam(idx, param, value)
}

export async function setEffectParam(item: EffectType, idx:number, param: number, value: number){
    console.log("EFFECT IDX", idx);
    await window.synthAPI.pipelineSetEffectParam(idx, param, value)
}

export async function deleteItem(idx: number) {
    await window.synthAPI.pipelineRemove(idx);
}

export async function swapItems(idx1: number, idx2: number) {
    console.log("idxs:", idx1, idx2)
    await window.synthAPI.pipelineSwap(idx1, idx2);
}


export async function moveItem(idx1: number, idx2:number){
    console.log("MOVE idxs:", idx1, idx2)
    await window.synthAPI.pipelineMove(idx1, idx2);
}


const cache = new Map<string, Promise<Point[]>>()

export function getComputedOscillator(oscName: string): Promise<Point[]> {
    if (!cache.has(oscName)) {
        cache.set(oscName,
            (async () => {
                let data: number[];
                try {
                    if (oscName == "meow") {
                        data = await window.synthAPI.getOscillatorPlot(oscName);
                    }
                } catch { data = [] }
                const dataPoints = data.map((y, x): Point => {
                    return { x, y };
                })
                return dataPoints;
            })()
        )
    }
    return cache.get(oscName)!;
}