import { z } from "zod"

export type blockType = {
    val: string;
    id: string;
    group: "filters" | "effects" | "empty"
}

export type listType = {
    val: string;
    group: "filters" | "effects";
}


export function getPosFromFiltered(list1: blockType[], idx: string) {
    const res: number = list1.filter(item => item.val !== "").findIndex(i => i.id === idx);
    return res;
}

export function getListFromData(data: FiltersData): listType[] {
    const dataFilters = data.filters.map((x, _): listType => { return { val: x, group: "filters" } });
    const dataEffects = data.effects.map((x, _): listType => { return { val: x, group: "effects" } });
    return dataFilters.concat(dataEffects);
}


export const FiltersFormSchema = z.object({
    filters: z.array(z.string()).max(3, {
        message: "You cannot select more than 3 filters.",
    }),
    effects: z.array(z.string()).max(3, {
        message: "You cannot select more than 3 effects.",
    })
})

export type FiltersData = z.infer<typeof FiltersFormSchema>