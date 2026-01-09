import {Button } from "@chakra-ui/react";
import { Opt, OptKey, OptEffectKey } from "@/app/utils/tables";
import { calcValueFromLogScale, calcValueFromLinScale,calcLinearPosFromLogarithmic, calcLogaritmicPosFromLinear, getBounds} from "@/app/utils/maths_utils";
import { ItemsParams, OptParams} from "@/app/utils/context_utils";
import { getOptParams } from "./SliderLinLog";
import { getData } from "./SliderLinLog";
import type { changeParams } from "@/app/sequencer/actions";

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
    
    const handleClick = async () => {
        const newStatus = status === "linear" ? "logarithmic" : "linear";
        const rec = (group == "filters") ? getOptParams(obj.params, optKey as OptKey) : getOptParams(obj.params, optKey as OptEffectKey);

        const sliderVal = rec.EndVal;
        const actProps = rec.Props;
        
        const res:changeParams = status === "linear" ? 
            await getData(true, "lin", group, sliderVal, obj.params.value, optKey, true, actProps.bounds) as changeParams
            :
            await getData(true, "log", group, sliderVal, obj.params.value, optKey, true) as changeParams;

        setStatus(newStatus);
        setSliderValue(itemID, optKey, "Val", res.newPos);
        setSliderValue(itemID, optKey, "EndVal", res.newPos);
        setSliderValue(itemID, optKey, "Props", {actValue:actProps.actValue, bounds: res.range});
    };

    return (
        <Button bg={status === "linear" ? "yellow.600" : "blue.600"}
            onClick={handleClick}>
            {status}
        </Button>
    )
}