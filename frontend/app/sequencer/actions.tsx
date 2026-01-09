"use server";
import fs from "fs/promises";
import { FILTER_TYPES } from "../utils/tables";

import { calcValueFromLogScale, calcValueFromLinScale,calcLinearPosFromLogarithmic,
    calcLogaritmicPosFromLinear, getBounds } from "@/app/utils/maths_utils";

import { defaultEffectOpts, EffectType, FilterType, Opt, OptEffectKey, OptKey, defaultOpts } from "../utils/tables";
const TEST_SLIDERS = 100;

export type changeParams = {
    range: number[];
    newPos: number; // only range and slider position changes
}

type sliderVals = {
    val: number; // numeric mapping slider pos to scale lin/log
    change?: changeParams, // if slider handles logarithmic scale
    // key in this record will be the lower bound 
}

export type sliderCache = {
    lin: {
        filters: Record<string, Record<string, sliderVals>>;
        effects: Record<string, Record<string, sliderVals>>;
    }
    log:
    {
        filters: Record<string, Record<string, sliderVals>>;
        effects: Record<string, Record<string, sliderVals>>;
    }
}


export async function writeData() {
  const outPath = "./app/sequencer/data.json";
  const data = await CalcSliders();
  await fs.writeFile(outPath, JSON.stringify(data, null, 2), "utf8");
}

export async function CalcSliders(){

    const data: sliderCache = {
        lin: {
            filters: {},
            effects: {},
        },
        log: {
            filters: {},
            effects: {},
        },
    };

    function roundData(val: number, bounds: number[]):number {
        const dist = bounds[1] - bounds[0];
        return (dist <= 0.005) ? Number.parseFloat(val.toFixed(4)) :
            (dist <= 2) ? Number.parseFloat(val.toFixed(2)) :
                ((dist <= 10) ? Number.parseFloat(val.toFixed(1)) : Math.round(val))
    }


    (Object.entries(defaultEffectOpts) as [EffectType, Record<string, Opt>][])
    .forEach(([name, rec], _) => {
        (Object.entries(rec) as [string, Opt][])
        .forEach(([param, opt], _) => {

            const tempLin: Record<string, sliderVals> = {} as Record<string, sliderVals>;
            const tempLog: Record<string, sliderVals> = {} as Record<string, sliderVals>;

            for (let i = 0; i < TEST_SLIDERS+1; i++){
                const i_str = i.toString();

                tempLin[i_str] ??= {} as sliderVals;
                tempLin[i_str].val = roundData(calcValueFromLinScale(i, opt.range), opt.range);


                if ("logScale" in opt && opt.logScale){
                    tempLog[i_str] ??= {} as sliderVals;
                    tempLog[i_str].val = roundData(calcValueFromLogScale(i, opt.range), opt.range);

                    const firstPow = Math.pow(10, Math.floor(Math.log10(opt.range[0])));

                    for(let num = firstPow; num <= opt.range[1]; num *= 10 ){ // all possible powers of 10

                        const bounds:number[] = [Math.max(num, opt.range[0]), Math.min(num * 10, opt.range[1])] 
                        
                        const stateKey = `${i}.${bounds[0]}`;
                        tempLin[stateKey] ??= {} as sliderVals;
                        tempLin[stateKey].val = calcValueFromLinScale(i, bounds);
                        tempLin[stateKey].change ??= {} as changeParams;
                        tempLin[stateKey].change = {range: opt.range, newPos: calcLogaritmicPosFromLinear(i, bounds, opt.range)};
                    }

                    const newBounds = getBounds(tempLog[i_str].val, opt.range);

                    // change from log
                    tempLog[i_str].change ??= {} as changeParams;
                    tempLog[i_str].change.range = newBounds;
                    tempLog[i_str].change.newPos = calcLinearPosFromLogarithmic(i, opt.range, newBounds);
                }
            }
            data.lin.effects ??= {} as Record<string, Record<string, sliderVals>>;
            data.log.effects ??= {} as Record<string, Record<string, sliderVals>>;

            data.lin.effects[`${name}.${param}`] = tempLin;
            data.log.effects[`${name}.${param}`] = tempLog;

        })
    });


    FILTER_TYPES.forEach((name:FilterType, _) => {
        (Object.entries(defaultOpts) as [OptKey, Opt][]).forEach(([param, opt], _) => {
            const tempLin: Record<string, sliderVals> = {} as Record<string, sliderVals>;
            const tempLog: Record<string, sliderVals> = {} as Record<string, sliderVals>;

            for (let i = 0; i < TEST_SLIDERS+1; i++){

                const i_str = i.toString();

                tempLin[i_str] ??= {} as sliderVals;
                tempLin[i_str].val = roundData(calcValueFromLinScale(i, opt.range), opt.range);

                if ("logScale" in opt && opt.logScale){
                    tempLog[i_str] ??= {} as sliderVals;
                    tempLog[i_str].val = roundData(calcValueFromLogScale(i, opt.range), opt.range);

                    const firstPow = Math.pow(10, Math.floor(Math.log10(opt.range[0])));

                    for(let num = firstPow; num <= opt.range[1]; num *= 10 ){ // all possible powers of 10

                        const bounds:number[] = [Math.max(num, opt.range[0]), Math.min(num * 10, opt.range[1])] 

                        const stateKey = `${i}.${bounds[0]}`;
                        tempLin[stateKey] ??= {} as sliderVals;
                        tempLin[stateKey].val = calcValueFromLinScale(i, bounds);
                        tempLin[stateKey].change ??= {} as changeParams;
                        tempLin[stateKey].change = {range: opt.range, newPos: calcLogaritmicPosFromLinear(i, bounds, opt.range)};
                    }

                    const newBounds = getBounds(tempLog[i_str].val, opt.range);

                    tempLog[i_str].change ??= {} as changeParams;
                    tempLog[i_str].change.range = newBounds;
                    tempLog[i_str].change.newPos = calcLinearPosFromLogarithmic(i, opt.range, newBounds);
                }
            }

            data.lin.filters[`${name}.${param}`]  ??= {} as Record<string, sliderVals>;
            data.log.filters[`${name}.${param}`] ??= {} as Record<string, sliderVals>;

            data.lin.filters[`${name}.${param}`] = tempLin;
            data.log.filters[`${name}.${param}`] = tempLog;

        })
    });

    return data;

}