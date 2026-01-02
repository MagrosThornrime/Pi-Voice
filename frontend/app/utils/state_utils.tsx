import { SliderProps } from "@/components/SlidersItems";
import {Opt, OptKey, Filter, defaultOpts, FilterType } from "../utils/tables"
import { FiltersParams, OptParams, useFiltersParams, useFiltersLogic } from "./context_utils";

export const buildInitialOptsState = (items:Filter[], initial:number= 0):Record<string, number>[] => {
    const state: Record<string, number> = {};

    return items.map((item, _) => {
        const state: Record<string, number> = {};
        (Object.entries(item.opts) as [OptKey, Opt][]).map(([key, _]) => {
            state[key] = initial;
        });
        return state;
    });
};


export const buildInitialPropsState = (items:Filter[], initial:number= 0):Record<string, SliderProps>[] => {
    const state: Record<string, number> = {};

    return items.map((item, _) => {
        const state: Record<string, SliderProps> = {};
        (Object.entries(item.opts) as [OptKey, Opt][]).map(([key, _]) => {
            state[key] = {bounds: defaultOpts[key].range, actValue: initial}
        });
        return state;
    });
};