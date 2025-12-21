import {Opt, OptKey, Filter, defaultOpts, } from "../utils/tables"
export const buildInitialState = (items:Filter[], ifEnd:boolean, initial:number|number[] = 0, ifBounds = false) => {
    const state: Record<string, any> = {};

    items.forEach(item => {
        (Object.entries(item.opts) as [OptKey, Opt][]).map(([key, opt]) => {
            if (!ifBounds){
                if (defaultOpts[key].mutable){
                    state[`${item.value}.${key}${ifEnd ? "_end" : ""}`] = [initial]
                }
                else{
                    state[`${item.value}.${key}${ifEnd ? "_end" : ""}`] = [50]
                }
            }
            else{
                if (defaultOpts[key].mutable){
                    state[`${item.value}.${key}`] = {bounds: defaultOpts[key].range, actValue: initial}
                }
                else{
                    state[`${item.value}.${key}`] = {bounds: defaultOpts[key].range, actValue: 50}
                }
            }
        });
    });
    return state;
};