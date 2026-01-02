import { Box, Button, Text, Collapsible, Flex, Grid, Slider } from "@chakra-ui/react";
import {useState, Fragment} from "react";
import { SliderProps } from "./SlidersItems";
import { Opt, OptKey, Filter, defaultOpts, OptEffectKey } from "@/app/utils/tables";
import { calcValueFromLogScale, calcValueFromLinScale,calcLinearPosFromLogarithmic, calcLogaritmicPosFromLinear, getBounds} from "@/app/utils/maths_utils";
import { setFilterParam } from "@/app/utils/integration_utils";
import { FiltersParams, ItemsParams, EffectsParams, OptParams} from "@/app/utils/context_utils";
import { getOptParams } from "./SliderLinLog";

type ButtonScaleProps<P extends keyof OptParams = keyof OptParams>  = {
    setSliderValue: (
        itemID: string,
        opt: OptKey | OptEffectKey,
        property: P,
        newValue: OptParams[P]
    ) => void;
    optKey: OptKey | OptEffectKey;
    opt: Opt;
    itemID: string;
    paramsData: ItemsParams[];
    status: string;
    setStatus: React.Dispatch<React.SetStateAction<string>>;
}


export function ButtonScale({ setStatus, status, setSliderValue, optKey, opt, itemID, paramsData }: ButtonScaleProps) {
    const obj = paramsData.find(f => f.id === itemID);
    if (!obj) return null;
    const group = obj.params.group;
    
    const handleClick = () => {
        const newStatus = status === "linear" ? "logarithmic" : "linear";
        const rec = (group == "filters") ? getOptParams(obj.params, optKey as OptKey) : getOptParams(obj.params, optKey as OptEffectKey);

        const sliderVal = rec.EndVal;
        const actProps = rec.Props;

        const logPos = calcLogaritmicPosFromLinear(
            sliderVal,
            actProps.bounds,
            opt.range
        );

        const actVal =
            newStatus === "logarithmic"
                ? calcValueFromLinScale(sliderVal, actProps.bounds)
                : calcValueFromLogScale(sliderVal, opt.range);

        const linRange = getBounds(actVal, opt.range);
        const linPos = calcLinearPosFromLogarithmic(
            sliderVal,
            opt.range,
            linRange
        );

        const nextPos = newStatus === "logarithmic" ? logPos : linPos;
        const nextProps =
            newStatus === "logarithmic"
                ? { bounds: opt.range, actValue: actVal }
                : { bounds: linRange, actValue: actVal };

        setStatus(newStatus);
        setSliderValue(itemID, optKey, "Val", nextPos);
        setSliderValue(itemID, optKey, "EndVal", nextPos);
        setSliderValue(itemID, optKey, "Props", nextProps);
    };

    return (
        <Button bg={status === "linear" ? "green.400" : "red.400"}
            onClick={handleClick}>
            {status}
        </Button>
    )
}