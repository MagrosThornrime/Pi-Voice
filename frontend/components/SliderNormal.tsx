import { Box, Text, Flex, Slider } from "@chakra-ui/react";
import { Fragment } from "react";
import { Opt, OptKey, defaultOpts } from "@/app/utils/tables";
import { calcValueFromLinScale } from "@/app/utils/maths_utils";
import { setFilterParam} from "@/app/utils/integration_utils";
import { FiltersParams } from "@/app/utils/context_utils";


type NormalSliderProps = {
    setSliderValue: (itemIdx: string, opt: OptKey, newValue: number) => void;
    setEndSliderValue: (itemIdx: string, opt: OptKey, newValue: number) => void;
    optKey: OptKey;
    itemID: string;
    paramsData:FiltersParams[];
    opt: Opt;

};

export function SliderNormal({ setSliderValue, setEndSliderValue, opt, optKey, paramsData, itemID}: NormalSliderProps) {
    const obj = paramsData.find(f => f.id === itemID);
    if (!obj) return null;

    const Value = obj.record[optKey].Val;

    return (
        <Fragment key={`${itemID}${optKey}`}>
            <Text mb={2} fontWeight="medium" color="white" textAlign="left"> {optKey} </Text>
            <Slider.Root
                value={[Value]}
                onValueChange={details => {
                    setSliderValue(itemID, optKey, details.value[0]);
                }}
                onValueChangeEnd={async details => {
                    const sliderVal = details.value[0];
                    const linVal = calcValueFromLinScale(sliderVal, opt.range)
                    setEndSliderValue(itemID, optKey, sliderVal);

                    console.log("FILTER PARAMS", itemID, defaultOpts[optKey].index, Math.round(linVal))

                    await setFilterParam(paramsData.findIndex((f) => f.id === itemID), defaultOpts[optKey].index, linVal );

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
                                obj.record[optKey].EndVal,
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