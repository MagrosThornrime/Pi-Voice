import { Box, Text, Flex, Slider, Center, Stack } from "@chakra-ui/react";
import { useState, Fragment } from "react";
import { Opt, OptKey, defaultOpts, OptEffectKey, defaultEffectOpts } from "@/app/utils/tables";
import { calcValueFromLogScale, calcValueFromLinScale } from "@/app/utils/maths_utils";
import { setEffectParam, setFilterParam } from "@/app/utils/integration_utils";
import { ButtonScale } from "./ButtonScale";
import { FiltersParams, ItemsParams, EffectsParams, OptParams} from "@/app/utils/context_utils";
import { SliderTooltip } from "./SliderTooltip";


type LogSliderProps<P extends keyof OptParams = keyof OptParams> = {
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

export function getOptParams(
  params: FiltersParams,
  key: OptKey
): OptParams
export function getOptParams(
  params: EffectsParams,
  key: OptEffectKey
): OptParams
export function getOptParams(
  params: FiltersParams | EffectsParams,
  key: OptKey | OptEffectKey
) {
  return params.group === "filters"
    ? params.record[key as OptKey]
    : params.record[key as OptEffectKey]
}


export function LogSlider({ setSliderValue, opt, optKey, itemID, paramsData}: LogSliderProps) {
    const obj = paramsData.find(f => f.id === itemID);
    if (!obj) return null;

    const group = obj.params.group;
    const rec = (group == "filters") ? getOptParams(obj.params, optKey as OptKey) : getOptParams(obj.params, optKey as OptEffectKey);
    

    const Value = rec.Val;
    const Props = rec.Props;

    
    const [status, setStatus] = useState<string>("logarithmic");

    return (
        <Fragment key={`${itemID}${optKey}`}>

           <Stack direction="row" w="100%" align="center">

                <Text fontWeight="medium"  color="white"> {optKey} </Text>
                <Box flex="1" display="flex" justifyContent="center">
                    <ButtonScale setStatus={setStatus} status={status}
                        optKey={optKey} setSliderValue={setSliderValue} 
                        paramsData={paramsData}
                        opt={opt} itemID = {itemID} />
                </Box>

                <Box minW="fit-content" visibility="hidden">
                    {optKey}
                </Box>

            </Stack>

            <Box h = "2"/>

            <Slider.Root
                value={[Value]}

                onValueChange={(details) => {
                    const sliderVal = details.value[0]
                    const rec = (group == "filters") ? getOptParams(obj.params, optKey as OptKey) : getOptParams(obj.params, optKey as OptEffectKey);
                    const actProps = rec.Props;

                    setSliderValue(itemID, optKey, "Val", sliderVal)
                    setSliderValue(itemID, optKey, "Props", {
                        bounds: actProps.bounds,
                        actValue: status === "logarithmic"
                            ? calcValueFromLogScale(sliderVal, opt.range)
                            : calcValueFromLinScale(sliderVal, actProps.bounds)
                    })
                }
                }


                onValueChangeEnd={async (details) => {
                    const sliderVal = details.value[0];

                    const rec = (group == "filters") ? getOptParams(obj.params, optKey as OptKey) : getOptParams(obj.params, optKey as OptEffectKey);
                    const actProps = rec.Props;

                    const newVal = status === "logarithmic"
                                ? (calcValueFromLogScale(sliderVal, opt.range))
                                : (calcValueFromLinScale(sliderVal, actProps.bounds))

                    setSliderValue(itemID, optKey, "EndVal", sliderVal);
                    setSliderValue( itemID, optKey, "Props", { bounds: actProps.bounds, actValue: newVal } )

                    console.log("SLIDER END VALUE: ", sliderVal);

                    if (group == "filters"){
                        const filterName = obj.params.value;
                        const optionIndex = defaultOpts[optKey as OptKey].index;
                        const filterIndex = paramsData.findIndex((f) => f.id === itemID);

                        console.log("FILTER LOG PARAM", filterIndex, optionIndex, Math.round(newVal))
                        await setFilterParam(filterName, filterIndex, optionIndex, newVal);
                    }
                    else{
                        const effectName = obj.params.value;
                        const optionIndex = defaultEffectOpts[effectName][optKey as OptEffectKey].index;
                        const effectIndex = paramsData.findIndex((f) => f.id === itemID);

                        console.log("EFFECT LOG PARAM", effectIndex, optionIndex, Math.round(newVal))
                        await setEffectParam(effectName, effectIndex, optionIndex, newVal);
                    }
                }}>


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