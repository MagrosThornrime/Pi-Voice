import { Box, Button, Text, Collapsible, Flex, Grid, Slider } from "@chakra-ui/react";
import {useState, Fragment} from "react";
import { SliderProps } from "./SlidersItems";
import { Opt, OptKey, Filter, defaultOpts } from "@/app/utils/tables";
import { calcValueFromLogScale, calcValueFromLinScale,calcLinearPosFromLogarithmic, calcLogaritmicPosFromLinear, getBounds} from "@/app/utils/maths_utils";
import { setFilterParam} from "@/app/effects_filters/page";
import { ButtonScale } from "./ButtonScale";


type LogSliderProps = {
    setSliderVal:(itemValue: string, opt: string, newValue: number) => void;
    setSliderEndVal: (itemValue: string, opt: string, newValue: number) => void;
    setSliderProps: (itemValue: string, opt: string, newProps: SliderProps) => void;
    opt_key: OptKey;
    EndValues: Record<string, any>;
    Values: Record<string, any>;
    Props: Record<string, SliderProps>;
    opt:Opt;
    obj: Filter;
    

};
export function LogSlider({ setSliderVal, setSliderEndVal, setSliderProps, obj, opt, opt_key, EndValues, Values, Props }: LogSliderProps) {
    const [status, setStatus] = useState<string>("logarithmic");
    const state_key = `${obj.value}.${opt_key}`;
    const Value = Values[state_key];
    return (
        <Fragment key={state_key}>
            <Slider.Root
                value={[Value]}

                onValueChange={(details) => {
                    const sliderVal = details.value[0]
                        setSliderVal(obj.value, opt_key, sliderVal)

                        setSliderProps(obj.value, opt_key, {
                            bounds: Props[state_key].bounds,
                            actValue: ("logScale" in opt && opt.logScale)
                                ? (status === "logarithmic"
                                    ? calcValueFromLogScale(sliderVal, opt.range)
                                    : calcValueFromLinScale(sliderVal, Props[state_key].bounds)
                                )
                                : calcValueFromLinScale(sliderVal, opt.range)
                        })
                }
                }


                onValueChangeEnd={async (details) => {
                    const sliderVal = details.value[0]
                        setSliderEndVal(obj.value, opt_key, sliderVal);

                        setSliderProps(obj.value, opt_key,
                            {
                                bounds: Props[`${state_key}`].bounds,
                                actValue: ("logScale" in opt && opt.logScale) ?
                                    (
                                        status === "logarithmic" ?
                                            (calcValueFromLogScale(sliderVal, opt.range))
                                            :
                                            (calcValueFromLinScale(sliderVal, Props[state_key].bounds))
                                    )
                                    :
                                    (calcValueFromLinScale(sliderVal, opt.range))

                            }
                        )

                    console.log("SLIDER END VALUE: ", EndValues[`${state_key}_end`]);
                    await setFilterParam(obj.value, defaultOpts[opt_key].index, Props[state_key].actValue);
                }}>

                <Slider.Label color="white"> {`${opt_key}`} </Slider.Label>
                <Slider.Control>
                    <Slider.Track>
                        <Slider.Range />
                    </Slider.Track>
                    <Slider.Thumbs />
                </Slider.Control>
            </Slider.Root>


            <Flex justify="space-between" align="center" mb={2} w="100%">

                <Text>
                    {Props[`${state_key}`].bounds[0]}
                </Text>

                <Text> Val:
                    {Math.round(Props[`${state_key}`].actValue)}
                </Text>

                <ButtonScale setStatus = {setStatus} status = {status}
                    opt_key={opt_key} setSliderVal={setSliderVal} setSliderEndVal={setSliderEndVal} Props={Props}
                    setSliderProps={setSliderProps} opt={opt} obj={obj} Values={Values} EndValues={EndValues} />

                <Text> {Props[`${state_key}`].bounds[1]} </Text>
            </Flex>

            <Box h="5" />

        </Fragment>
    )
}