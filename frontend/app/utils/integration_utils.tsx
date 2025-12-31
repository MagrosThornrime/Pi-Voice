import { filters } from "./tables";

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

export async function addFilter(item: string, idx: number) {
    const filterNumber = filters.findIndex(i => i.value === item);
    await window.synthAPI.pipelineAddFilter(filterNumber, idx);
}

export async function setFilterParam(idx:number, param: number, value: number) {
    console.log("FILTER IDX", idx);
    await window.synthAPI.pipelineSetFilterParam(idx, param, value)
}

export async function deleteFilter(idx: number) {
    await window.synthAPI.pipelineRemove(idx);
}

export async function swapFilters(idx1: number, idx2: number) {
    console.log("idxs:", idx1, idx2)
    await window.synthAPI.pipelineSwap(idx1, idx2);
}


export async function moveFilter(idx1: number, idx2:number){
    console.log("MOVE idxs:", idx1, idx2)
    await window.synthAPI.pipelineMove(idx1, idx2);
}