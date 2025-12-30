import { Box, Text, Flex, Slider } from "@chakra-ui/react";
import { useState, Fragment } from "react";
import { SliderProps } from "./SlidersItems";
import { Opt, OptKey, Filter, defaultOpts } from "@/app/utils/tables";
import { calcValueFromLogScale, calcValueFromLinScale } from "@/app/utils/maths_utils";
import { setFilterParam } from "@/app/effects_filters/page";
import { ButtonScale } from "./ButtonScale";
import { FiltersParams } from "@/app/utils/context_utils";


type LogSliderProps = {
    setSliderVal: (itemIdx: string, opt: string, newValue: number) => void;
    setSliderEndVal: (itemIdx: string, opt: string, newValue: number) => void;
    setSliderProps: (itemIdx: string, opt: string, newValue: SliderProps) => void;
    optKey: OptKey;
    opt: Opt;
    itemID: string;
    paramsData:FiltersParams[];


};
export function LogSlider({ setSliderVal, setSliderEndVal, setSliderProps, opt, optKey, itemID, paramsData}: LogSliderProps) {
    const obj = paramsData.find(f => f.id === itemID);
    if (!obj) return null;

    const [status, setStatus] = useState<string>("logarithmic");
    const state_key = `${obj.value}.${optKey}`;
    const Value = obj.record[optKey].Val;

    return (
        <Fragment key={`${itemID}${optKey}`}>

            <Flex
                align="center"
                justify="center"
                w="100%"
            >

                <Text fontWeight="medium" color="white"> {optKey} </Text>
                <Box flex="1" display="flex" justifyContent="center">
                    <ButtonScale setStatus={setStatus} status={status}
                        optKey={optKey} setSliderVal={setSliderVal} setSliderEndVal={setSliderEndVal}
                        setSliderProps={setSliderProps} paramsData={paramsData}
                        opt={opt} itemID = {itemID} />
                </Box>

            </Flex>

            <Slider.Root
                value={[Value]}

                onValueChange={(details) => {
                    const sliderVal = details.value[0]
                    const actProps = obj.record[optKey].Props;
                    setSliderVal(itemID, optKey, sliderVal)
                    setSliderProps(itemID, optKey, {
                        bounds: actProps.bounds,
                        actValue: status === "logarithmic"
                            ? calcValueFromLogScale(sliderVal, opt.range)
                            : calcValueFromLinScale(sliderVal, actProps.bounds)
                    })
                }
                }


                onValueChangeEnd={async (details) => {
                    const sliderVal = details.value[0];
                    const actProps = obj.record[optKey].Props;

                    const newVal = status === "logarithmic"
                                ? (calcValueFromLogScale(sliderVal, opt.range))
                                : (calcValueFromLinScale(sliderVal, actProps.bounds))

                    setSliderEndVal(itemID, optKey, sliderVal);
                    setSliderProps( itemID, optKey,{ bounds: actProps.bounds,actValue: newVal } )

                    console.log("SLIDER END VALUE: ", obj.record[optKey].EndVal);

                    await setFilterParam(paramsData.findIndex((f) => f.id === itemID), defaultOpts[optKey].index, newVal);
                    console.log("FILTER LOG PARAM", obj.value, defaultOpts[optKey].index, Math.round(newVal))
                }}>


                <Slider.Control>
                    <Slider.Track>
                        <Slider.Range />
                    </Slider.Track>
                    <Slider.Thumbs />
                </Slider.Control>
            </Slider.Root>


            <Flex justify="space-between" align="center" mb={2} w="100%">

                <Text>
                    {obj.record[optKey].Props.bounds[0]}
                </Text>
                <Box flex="1" textAlign="center" minW="0">
                    <Text> Val:
                        {Math.round(obj.record[optKey].Props.actValue)}
                    </Text>
                </Box>

                <Text> {obj.record[optKey].Props.bounds[1]} </Text>
            </Flex>

            <Box h="5" />

        </Fragment>
    )
}