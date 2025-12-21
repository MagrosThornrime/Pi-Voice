import { Box, Checkbox, Button, Fieldset, Stack, Text, CheckboxGroup, Heading, Collapsible, Flex, Grid, Slider } from "@chakra-ui/react";
import { useContext, ReactNode, useEffect, createContext, useState, Fragment} from "react";
import { standardSchemaResolver } from "@hookform/resolvers/standard-schema"
import { useController, ControllerRenderProps, useForm, FieldError } from "react-hook-form"
import { LuChevronRight } from "react-icons/lu"
import {calcLinearPosFromLogarithmic, getBounds, calcLogaritmicPosFromLinear, calcValueFromLogScale, calcValueFromLinScale} from "../app/utils/maths_utils"
import {Opt, OptKey, Filter, filters, effects, defaultOpts, } from "../app/utils/tables"
import { buildInitialState } from "../app/utils/state_utils";
import { z } from "zod"
import { useFilters, setFilterParam } from "@/app/effects_filters/page";


type SliderProps = {
    bounds: number[];
    actValue: number;
}


type SlidersItemsProps = {
    neededItems: Filter[];
    attr: "filters" | "effects";
};

export function SlidersItems({ neededItems, attr }: SlidersItemsProps) {

    const { data } = useFilters();

    const filteredItems = neededItems.filter(item =>
        data[attr].includes(item.value)
    );

    const [Values, setValues] = useState<Record<string, any>>(buildInitialState(neededItems, false, 0));
    const [EndValues, setEndValues] = useState<Record<string, any>>(buildInitialState(neededItems, true, 0));
    const [Props, setProps] = useState<Record<string, SliderProps>>(buildInitialState(neededItems, false, 0, true));

    const setSliderValue = (itemValue: string, opt: string, newValue: number) => {
        setValues(prev => ({
            ...prev,
            [`${itemValue}.${opt}`]: newValue
        }));
    };

    const setEndSliderValue = (itemValue: string, opt: string, newValue: number) => {
        setEndValues(prev => ({
            ...prev,
            [`${itemValue}.${opt}_end`]: newValue
        }));
    };

    const setSliderProps= (itemValue: string, opt: string, newProps: SliderProps) => {
        setProps(prev => ({
            ...prev,
            [`${itemValue}.${opt}`]: newProps
        }))
    }

    useEffect(() => {
        console.log("EndValues changed:", EndValues);
    } , [EndValues]);

    const [status, setStatus] = useState<string>("logarithmic");

    const sliders = filteredItems.flatMap(obj => {
        return (
            <Fragment key={obj.value}>
                <Box p={5} bg="grey" rounded="2xl" maxW="100%" shadow="md">
                    <Text mb={2} fontWeight="medium" textAlign="center">{obj.value}</Text>
                    {
                        ( Object.entries(obj.opts) as [OptKey, Opt][]).map(([key, opt]) => {
                            const opt_key = key;
                            const state_key = `${obj.value}.${opt_key}`;
                            const Value = Values[state_key];
                            console.log(Values);
                            console.log(obj);
                            return (
                                <Fragment key={`${obj.value}.${opt_key}`}>

                                    <Slider.Root
                                        key={state_key}
                                        value={[Value]}

                                        onValueChange={(details) => {
                                            const sliderVal = details.value[0]
                                            if (opt.mutable) {
                                                setSliderValue(obj.value, opt_key, sliderVal)

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
                                        }


                                        onValueChangeEnd={async (details) => {
                                            const sliderVal = details.value[0]
                                            if (opt.mutable) {
                                                setEndSliderValue(obj.value, opt_key, sliderVal);

                                                setSliderProps(obj.value, opt_key,
                                                    {
                                                        bounds: Props[`${state_key}`].bounds,
                                                        actValue: ("logScale" in opt && opt.logScale) ?
                                                            (
                                                                status === "logarithmic" ?
                                                                    ( calcValueFromLogScale(sliderVal, opt.range) )
                                                                    :
                                                                    ( calcValueFromLinScale(sliderVal, Props[state_key].bounds) )
                                                            )
                                                            :
                                                            ( calcValueFromLinScale(sliderVal, opt.range) )

                                                    }
                                                )
                                            }

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
                                            {opt.mutable && Props[`${state_key}`].bounds[0]}
                                        </Text>

                                        <Text> Val:
                                            {
                                                opt.mutable ? Math.round(Props[`${state_key}`].actValue) : opt.range[0] 
                                            } 
                                        </Text> 

                                        {
                                           "logScale" in opt && opt.logScale &&
                                           <Button bg={status === "linear" ? "green.400" : "red.400"}
                                                onClick={() => {
                                                    setStatus(prev => {
                                                        const newStatus = prev === "linear" ? "logarithmic" : "linear";
                                                        const SliderVal = EndValues[`${state_key}_end`]

                                                        let logPos = calcLogaritmicPosFromLinear(SliderVal, Props[state_key].bounds, opt.range)

                                                        let actVal = newStatus === "logarithmic" ? 
                                                        ( calcValueFromLinScale(SliderVal, Props[state_key].bounds))
                                                        :
                                                        ( calcValueFromLogScale(SliderVal, opt.range) )

                                                        let linRange = getBounds(actVal, opt.range);
                                                        let linPos = calcLinearPosFromLogarithmic(SliderVal, opt.range, linRange);
                                                        
                                                        setSliderValue(
                                                            obj.value, opt_key,
                                                            newStatus === "logarithmic" ? logPos : linPos
                                                        );

                                                        setEndSliderValue(
                                                            obj.value, opt_key,
                                                            newStatus === "logarithmic" ? logPos : linPos
                                                        );

                                                        setSliderProps(
                                                            obj.value, opt_key,
                                                            newStatus === "logarithmic" ? 
                                                            ({ bounds: opt.range, actValue: actVal })
                                                            :
                                                            ({ bounds: linRange, actValue: actVal })
                                                        );

                                                        return newStatus;
                                                    })
                                                }
                                            }>
                                            { status }
                                            </Button>
                                        }

                                        <Text> {opt.mutable && Props[`${state_key}`].bounds[1]} </Text>
                                    </Flex>

                                    <Box h="5" />

                                </Fragment>
                            );
                        })
                    }
                </Box>
            </Fragment>
        )
    });

    return (
        <Box>
            <Collapsible.Root defaultOpen justifyItems={"center"}>

                <Collapsible.Trigger paddingY="3" display="flex" gap="2" alignItems="center" justifyItems={"center"}>

                    <Collapsible.Indicator
                        transition="transform 0.2s"
                        _open={{ transform: "rotate(90deg)" }} >
                        <LuChevronRight size={24} color="black" />
                    </Collapsible.Indicator>

                    <Box maxW="100%">
                        <Text textStyle="2xl" mb={2} color="teal.600" fontWeight="semibold" textAlign="center"> Toggle {attr} </Text>
                    </Box>

                </Collapsible.Trigger>

                <Collapsible.Content maxW="100%" minW="100%">

                    <Grid templateColumns={{ base: "1fr", md: "repeat(3, 1fr)", lg: "repeat(3, 1fr)", }} gap={10} maxW="1000px" mx="auto">
                        {sliders}
                    </Grid>

                </Collapsible.Content>

            </Collapsible.Root>

        </Box>
    )

}