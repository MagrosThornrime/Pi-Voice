import { Box, Text,  Slider } from "@chakra-ui/react";
import { Fragment } from "react";
import { Opt, OptEffectKey, OptKey, defaultOpts, defaultEffectOpts } from "@/app/utils/tables";
import { setFilterParam, setEffectParam} from "@/app/utils/integration_utils";
import { ItemsParams } from "@/app/utils/context_utils";
import { getOptParams } from "./SliderLinLog";
import { OptParams } from "@/app/utils/context_utils";
import { SliderTooltip } from "./SliderTooltip";
import { getData } from "./SliderLinLog";

type NormalSliderProps<P extends keyof OptParams = keyof OptParams> = {
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

};

export function SliderNormal({ setSliderValue,  opt, optKey, paramsData, itemID}: NormalSliderProps) {
    const obj = paramsData.find(f => f.id === itemID);
    if (!obj) return null;

    const group = obj.params.group;
    const rec = (group == "filters") ? getOptParams(obj.params, optKey as OptKey) : getOptParams(obj.params, optKey as OptEffectKey);
    const Value = rec.Val;
    const Props = rec.Props;

    return (
        <Fragment key={`${itemID}${optKey}`}>
            <Text mb={2} fontWeight="medium" fontSize = "lg" color="white" textAlign="left"> {optKey} </Text>
            <Slider.Root
                value={[Value]}


                onValueChange={async (details) => {
                    const sliderVal = details.value[0];

                    const linVal = await getData(false, "lin", group, sliderVal, obj.params.value, optKey, false) as number;

                    setSliderValue(itemID, optKey, "Val", sliderVal);
                    setSliderValue( itemID, optKey, "Props", { bounds: opt.range, actValue: linVal } )
                } }


                onValueChangeEnd={async details => {
                    const sliderVal = details.value[0];

                    const linVal = await getData(false, "lin", group, sliderVal, obj.params.value, optKey, false) as number;

                    setSliderValue(itemID, optKey, "EndVal", sliderVal);
                    setSliderValue( itemID, optKey, "Props", { bounds: opt.range, actValue: linVal } )

                    if (group == "filters"){
                        const filterName = obj.params.value;
                        const optionIndex = defaultOpts[optKey as OptKey].index;
                        const filterIndex = paramsData.findIndex((f) => f.id === itemID);

                        console.log("FILTER PARAMS", filterIndex, optionIndex, Math.round(linVal))
                        await setFilterParam(filterName, filterIndex, optionIndex, linVal);
                    }
                    else{
                        const effectName = obj.params.value;
                        const optionIndex = defaultEffectOpts[effectName][optKey as OptEffectKey].index;
                        const effectIndex = paramsData.findIndex((f) => f.id === itemID);

                        console.log("EFFECT PARAM", effectIndex, optionIndex, Math.round(linVal))
                        await setEffectParam(effectName, effectIndex, optionIndex, linVal);
                    }

                } }
            >

                <Slider.Control>
                    <Slider.Track>
                        <Slider.Range />
                    </Slider.Track>
                    <Slider.Thumbs />
                </Slider.Control>
            </Slider.Root>

            <SliderTooltip Props = {Props} />

            <Box h="5" />
        </Fragment>
    )
}