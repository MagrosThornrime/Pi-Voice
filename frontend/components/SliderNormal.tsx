import { Box, Text, Flex, Slider } from "@chakra-ui/react";
import { Fragment } from "react";
import { Opt, OptEffectKey, OptKey, defaultOpts } from "@/app/utils/tables";
import { calcValueFromLinScale } from "@/app/utils/maths_utils";
import { setFilterParam} from "@/app/utils/integration_utils";
import { EffectsParams, FiltersParams, ItemsParams } from "@/app/utils/context_utils";
import { getOptParams } from "./SliderLinLog";
import { OptParams } from "@/app/utils/context_utils";


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
    const EndValue = rec.EndVal;

    return (
        <Fragment key={`${itemID}${optKey}`}>
            <Text mb={2} fontWeight="medium" color="white" textAlign="left"> {optKey} </Text>
            <Slider.Root
                value={[Value]}
                onValueChange={details => {
                    setSliderValue(itemID, optKey, "Val", details.value[0]);
                }}
                onValueChangeEnd={async details => {
                    const sliderVal = details.value[0];
                    const linVal = calcValueFromLinScale(sliderVal, opt.range)
                    setSliderValue(itemID, optKey, "EndVal", sliderVal);

                    console.log("FILTER PARAMS", itemID, defaultOpts[optKey as OptKey].index, Math.round(linVal))

                    await setFilterParam(paramsData.findIndex((f) => f.id === itemID), defaultOpts[optKey as OptKey].index, linVal );

                }}
            >

                <Slider.Control>
                    <Slider.Track>
                        <Slider.Range />
                    </Slider.Track>
                    <Slider.Thumbs />
                </Slider.Control>
            </Slider.Root>

            <Flex justify="center" align="center" w="100%">

                <Text>{opt.range[0]}</Text>

                <Box flex="1" textAlign="center" minW="0">
                    <Text>
                        Val:{" "}
                        {Math.round(
                            calcValueFromLinScale(
                                EndValue,
                                opt.range
                            )
                        )}
                    </Text>
                </Box>

                <Text>{opt.range[1]}</Text>

            </Flex>

            <Box h="5" />
        </Fragment>
    )
}