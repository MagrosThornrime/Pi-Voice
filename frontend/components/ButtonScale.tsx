import { Box, Button, Text, Collapsible, Flex, Grid, Slider } from "@chakra-ui/react";
import {useState, Fragment} from "react";
import { SliderProps } from "./SlidersItems";
import { Opt, OptKey, Filter, defaultOpts } from "@/app/utils/tables";
import { calcValueFromLogScale, calcValueFromLinScale,calcLinearPosFromLogarithmic, calcLogaritmicPosFromLinear, getBounds} from "@/app/utils/maths_utils";
import { setFilterParam} from "@/app/effects_filters/page";

type ButtonScaleProps = {
    setSliderVal:(itemValue: string, opt: string, newValue: number) => void;
    setSliderEndVal: (itemValue: string, opt: string, newValue: number) => void;
    setSliderProps: (itemValue: string, opt: string, newProps: SliderProps) => void;
    opt_key: OptKey;
    EndValues: Record<string, any>;
    Values: Record<string, any>;
    Props: Record<string, SliderProps>;
    opt:Opt;
    obj: Filter;
    status: string;
    setStatus: React.Dispatch<React.SetStateAction<string>>;
}


export function ButtonScale({ setStatus, status, setSliderVal, setSliderEndVal, setSliderProps, opt_key, obj, opt, Props, EndValues }: ButtonScaleProps) {
    const state_key = `${obj.value}.${opt_key}`;

    const handleClick = () => {
        const newStatus = status === "linear" ? "logarithmic" : "linear";
        const sliderVal = EndValues[`${state_key}_end`];

        const logPos = calcLogaritmicPosFromLinear(
            sliderVal,
            Props[state_key].bounds,
            opt.range
        );

        const actVal =
            newStatus === "logarithmic"
                ? calcValueFromLinScale(sliderVal, Props[state_key].bounds)
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
        setSliderVal(obj.value, opt_key, nextPos);
        setSliderEndVal(obj.value, opt_key, nextPos);
        setSliderProps(obj.value, opt_key, nextProps);
    };

    return (
        <Button bg={status === "linear" ? "green.400" : "red.400"}
            onClick={handleClick}>
            {status}
        </Button>
    )
}