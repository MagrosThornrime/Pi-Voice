import {Button } from "@chakra-ui/react";
import { Opt, OptKey, OptEffectKey } from "@/app/utils/tables";
import { calcValueFromLogScale, calcValueFromLinScale,calcLinearPosFromLogarithmic, calcLogaritmicPosFromLinear, getBounds} from "@/app/utils/maths_utils";
import { ItemsParams, OptParams} from "@/app/utils/context_utils";
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
        <Button bg={status === "linear" ? "yellow.600" : "blue.600"}
            onClick={handleClick}>
            {status}
        </Button>
    )
}