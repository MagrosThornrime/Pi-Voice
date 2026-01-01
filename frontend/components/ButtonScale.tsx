import { Box, Button, Text, Collapsible, Flex, Grid, Slider } from "@chakra-ui/react";
import {useState, Fragment} from "react";
import { SliderProps } from "./SlidersItems";
import { Opt, OptKey, Filter, defaultOpts } from "@/app/utils/tables";
import { calcValueFromLogScale, calcValueFromLinScale,calcLinearPosFromLogarithmic, calcLogaritmicPosFromLinear, getBounds} from "@/app/utils/maths_utils";
import { setFilterParam} from "@/app/effects_filters/page";
import { FiltersParams } from "@/app/utils/context_utils";

type ButtonScaleProps = {
    setSliderVal: (itemIdx: string, opt: string, newValue: number) => void;
    setSliderEndVal: (itemIdx: string, opt: string, newValue: number) => void;
    setSliderProps: (itemIdx: string, opt: string, newValue: SliderProps) => void;
    optKey: OptKey;
    opt:Opt;
    status: string;
    itemID:string;
    paramsData: FiltersParams[];
    setStatus: React.Dispatch<React.SetStateAction<string>>;
}


export function ButtonScale({ setStatus, status, setSliderVal, setSliderEndVal, setSliderProps, optKey, opt, itemID, paramsData }: ButtonScaleProps) {
    const obj = paramsData.find(f => f.id === itemID);
    if (!obj) return null;

    const state_key = `${obj.value}.${optKey}`;

    const handleClick = () => {
        const newStatus = status === "linear" ? "logarithmic" : "linear";
        const sliderVal = obj.record[optKey].EndVal;
        const actProps = obj.record[optKey].Props;

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
        setSliderVal(itemID, optKey, nextPos);
        setSliderEndVal(itemID, optKey, nextPos);
        setSliderProps(itemID, optKey, nextProps);
    };

    return (
        <Button bg={status === "linear" ? "green.400" : "red.400"}
            onClick={handleClick}>
            {status}
        </Button>
    )
}