import { Box, Text, Flex, Slider } from "@chakra-ui/react";
import { Fragment } from "react";
import { Opt, OptKey, Filter, defaultOpts } from "@/app/utils/tables";
import { calcValueFromLinScale } from "@/app/utils/maths_utils";
import { setFilterParam } from "@/app/effects_filters/page";
import { useOrderedFilters } from "@/app/utils/context_utils";


type NormalSliderProps = {
    setSliderValue: (itemValue: string, opt: string, newValue: number) => void;
    setEndSliderValue: (itemValue: string, opt: string, newValue: number) => void;
    optKey: OptKey;
    EndValues: Record<string, any>;
    Values: Record<string, any>;
    opt: Opt;
    obj: Filter;
    idx: number;
};

export function SliderNormal({ setSliderValue, setEndSliderValue, obj, opt, optKey, EndValues, Values, idx }: NormalSliderProps) {
    const stateKey = `${obj.value}.${optKey}`;
    const Value = Values[stateKey];
    const { orderedData } = useOrderedFilters();

    return (
        <Fragment key={`${stateKey}${idx}`}>
            <Text mb={2} fontWeight="medium" color="white" textAlign="left"> {optKey} </Text>
            <Slider.Root
                value={[Value]}
                onValueChange={details => {
                    setSliderValue(obj.value, optKey, details.value[0]);
                }}
                onValueChangeEnd={async details => {
                    const sliderVal = details.value[0];
                    const linVal = calcValueFromLinScale(sliderVal, opt.range)
                    setEndSliderValue(obj.value, optKey, sliderVal);

                    console.log("FILTER PARAMS", obj.value, defaultOpts[optKey].index, Math.round(linVal))

                    await setFilterParam(
                        orderedData.filters,
                        obj.value,
                        defaultOpts[optKey].index,
                        linVal
                    );

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
                                EndValues[`${stateKey}_end`],
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