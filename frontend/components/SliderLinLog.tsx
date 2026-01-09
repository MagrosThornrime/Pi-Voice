import { Box, Text, Flex, Slider, Center, Stack } from "@chakra-ui/react";
import { useState, Fragment } from "react";
import { Opt, OptKey, defaultOpts, OptEffectKey, defaultEffectOpts, FilterType, EffectType } from "@/app/utils/tables";
import { calcValueFromLogScale, calcValueFromLinScale } from "@/app/utils/maths_utils";
import { setEffectParam, setFilterParam } from "@/app/utils/integration_utils";
import { ButtonScale } from "./ButtonScale";
import { FiltersParams, ItemsParams, EffectsParams, OptParams} from "@/app/utils/context_utils";
import { SliderTooltip } from "./SliderTooltip";
import { GetChangesData, changeParams} from "@/app/sequencer/actions";

export async function getData(withChange: boolean, sliderType: "lin" | "log", itemType: "filters" | "effects", sliderVal: number,
    itemName: EffectType | FilterType, paramName:OptEffectKey | OptKey, change:boolean, bounds?: number[]
): Promise<changeParams | number>{

    if (withChange){
        return (await GetChangesData(sliderType, itemType, sliderVal, itemName, paramName, change, bounds)) as changeParams;
    }
    else{
        return (await GetChangesData(sliderType, itemType, sliderVal, itemName, paramName, change, bounds)) as number;
    }
}


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

                <Text fontWeight="medium" fontSize = "lg" color="white"> {optKey} </Text>
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

                onValueChange={async (details) => {
                    const sliderVal = details.value[0]
                    const rec = (group == "filters") ? getOptParams(obj.params, optKey as OptKey) : getOptParams(obj.params, optKey as OptEffectKey);
                    const actProps = rec.Props;
                    
                    const actValue = status === "logarithmic" ?
                        await getData(false,"log", group,sliderVal,obj.params.value,optKey,false ) as number
                        :
                        await getData(false,"lin",group,sliderVal, obj.params.value, optKey, false, actProps.bounds) as number;

                    setSliderValue(itemID, optKey, "Val", sliderVal)
                    setSliderValue(itemID, optKey, "Props", {
                        bounds: actProps.bounds,
                        actValue: actValue
                    })
                } }


                onValueChangeEnd={async (details) => {
                    const sliderVal = details.value[0];

                    const rec = (group == "filters") ? getOptParams(obj.params, optKey as OptKey) : getOptParams(obj.params, optKey as OptEffectKey);
                    const actProps = rec.Props;

                    const newVal = status === "logarithmic" ?
                        await getData(false,"log", group,sliderVal,obj.params.value,optKey,false ) as number
                        :
                        await getData(false,"lin",group,sliderVal, obj.params.value, optKey, false, actProps.bounds) as number;

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