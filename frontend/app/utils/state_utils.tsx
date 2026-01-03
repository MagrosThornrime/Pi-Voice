import { SliderProps } from "@/components/SlidersItems";
import {Opt, OptKey, Filter, defaultOpts } from "../utils/tables"

export const buildInitialOptsState = (items:Filter[], initial:number= 0):Record<OptKey, number>[] => {
    const state: Record<string, number> = {};

    return items.map((item, _) => {
        const state: Record<OptKey, number> = {} as Record<OptKey, number>;
        (Object.entries(item.opts) as [OptKey, Opt][]).map(([key, _]) => {
            state[key] = initial;
        });
        return state;
    });
};


export const buildInitialPropsState = (items:Filter[], initial:number= 0):Record<OptKey, SliderProps>[] => {
    const state: Record<string, number> = {};

    return items.map((item, _) => {
        const state: Record<OptKey, SliderProps> = {} as Record<OptKey, SliderProps>;
        (Object.entries(item.opts) as [OptKey, Opt][]).map(([key, _]) => {
            state[key] = {bounds: defaultOpts[key].range, actValue: initial}
        });
        return state;
    });
};