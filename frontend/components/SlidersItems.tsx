import { Box, Button, Text, Collapsible, Flex, Grid, Slider } from "@chakra-ui/react";
import {useEffect, useState, Fragment} from "react";
import { LuChevronRight } from "react-icons/lu"
import {calcLinearPosFromLogarithmic, getBounds, calcLogaritmicPosFromLinear, calcValueFromLogScale, calcValueFromLinScale} from "../app/utils/maths_utils"
import {Opt, OptKey, Filter, defaultOpts, filters } from "../app/utils/tables"
import { buildInitialState } from "../app/utils/state_utils";
import { useFilters, setFilterParam, useOrderedFilters, orderedDataType } from "@/app/effects_filters/page";
import { LogSlider } from "./SliderLinLog";
import { OrderSwitch } from "./OrderSwitch";


export type SliderProps = {
    bounds: number[];
    actValue: number;
}


type SlidersItemsProps = {
    neededItems: Filter[];
    attr: "filters" | "effects";
};

export function SlidersItems({ neededItems, attr }: SlidersItemsProps) {

  const { orderedData } = useOrderedFilters();

  const filteredItems = orderedData[attr].map((s: string) => {
    return (filters.filter((f: Filter) => {
      return f.value === s || f.label === s;
    }))[0] // because there will always be one element matching
  });

    console.log(filteredItems);
    
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

  const sliders = filteredItems.map(obj => (
  <Fragment key={obj.value}>
    <Box
      p={5}
      bg="grey"
      rounded="2xl"
      maxW="100%"
      shadow="md"
    >
      <Text mb={2} fontWeight="medium" textAlign="center">
        {obj.value}
      </Text>

      {(Object.entries(obj.opts) as [OptKey, Opt][]).map(([optKey, opt]) => {
        const stateKey = `${obj.value}.${optKey}`;
        const value = Values[stateKey];

        return (
          <Fragment key={stateKey}>
            {"logScale" in opt && opt.logScale ? (
              <LogSlider
                setSliderVal={setSliderValue}
                setSliderEndVal={setEndSliderValue}
                Props={Props}
                setSliderProps={setSliderProps}
                opt_key={optKey}
                Values={Values}
                EndValues={EndValues}
                obj={obj}
                opt={opt}
              />
            ) : !("step" in opt) ? (
              <>
                <Slider.Root
                  value={[value]}
                  onValueChange={details => {
                    setSliderValue(obj.value, optKey, details.value[0]);
                  }}
                  onValueChangeEnd={async details => {
                    const sliderVal = details.value[0];
                    setEndSliderValue(obj.value, optKey, sliderVal);

                    await setFilterParam(
                      obj.value,
                      defaultOpts[optKey].index,
                      calcValueFromLinScale(sliderVal, opt.range)
                    );
                  }}
                >
                  <Slider.Label color="white">
                    {optKey}
                  </Slider.Label>

                  <Slider.Control>
                    <Slider.Track>
                      <Slider.Range />
                    </Slider.Track>
                    <Slider.Thumbs />
                  </Slider.Control>
                </Slider.Root>

                <Flex justify="space-between" align="center" mb={2} w="100%">
                  <Text>{opt.range[0]}</Text>
                  <Text>
                    Val:{" "}
                    {Math.round(
                      calcValueFromLinScale(
                        EndValues[`${stateKey}_end`],
                        opt.range
                      )
                    )}
                  </Text>
                  <Text>{opt.range[1]}</Text>
                </Flex>

                <Box h="5" />
              </>
            ) : 
            (
              <>
                <OrderSwitch label={optKey} />
                <Box h="5" />
              </>
            )}
          </Fragment>
        );
      })}
    </Box>
  </Fragment>
));

    return (
        <Box>
            <Collapsible.Root justifyItems={"left"}>

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

                    <Grid templateColumns={{ base: "1fr", md: "repeat(2, 1fr)", lg: "repeat(3, 1fr)", }} gap={10} maxW="1000px" mx="auto">
                        {sliders}
                    </Grid>

                </Collapsible.Content>

            </Collapsible.Root>

        </Box>
    )

}