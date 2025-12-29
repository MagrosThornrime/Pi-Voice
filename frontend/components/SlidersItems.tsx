import { Box, Text, Collapsible, Grid } from "@chakra-ui/react";
import { useEffect, useState, Fragment } from "react";
import { LuChevronRight } from "react-icons/lu"
import { Opt, OptKey, Filter, filters } from "../app/utils/tables"
import { buildInitialState } from "../app/utils/state_utils";
import { useOrderedFilters} from "@/app/effects_filters/page";
import { LogSlider } from "./SliderLinLog";
import { OrderSwitch } from "./OrderSwitch";
import { SliderNormal } from "./SliderNormal";


export type SliderProps = {
  bounds: number[];
  actValue: number;
}


type SlidersItemsProps = {
  neededItems: Filter[];
  attr: "filters" | "effects";
  idx: number;
};

export function SlidersItems({ neededItems, attr, idx }: SlidersItemsProps) {

  const { orderedData } = useOrderedFilters();

  const filteredItems = orderedData[attr].map((s: string) => {
    return (filters.filter((f: Filter) => {
      return f.value === s || f.label === s;
    }))[0] // because there will always be one element matching
  });

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

  const setSliderProps = (itemValue: string, opt: string, newProps: SliderProps) => {
    setProps(prev => ({
      ...prev,
      [`${itemValue}.${opt}`]: newProps
    }))
  }

  useEffect(() => {
    console.log("EndValues changed:", EndValues);
  }, [EndValues]);

  const sliders = filteredItems.map((obj, idx1) => (
    <Fragment key={idx1}>
      <Box
        p={5}
        bg="grey"
        rounded="2xl"
        shadow="md"
      >
        <Text mb={2} fontWeight="medium" textAlign="center">
          {obj.value}
        </Text>

        {(Object.entries(obj.opts) as [OptKey, Opt][]).map(([optKey, opt], idxMap) => {
          const stateKey = `${obj.value}.${optKey}`;
          const value = Values[stateKey];

          return (
            <Fragment key={`${stateKey}${idxMap}`}>
              {
                "logScale" in opt && opt.logScale ? (

                  <LogSlider setSliderVal={setSliderValue}setSliderEndVal={setEndSliderValue}
                    Props={Props} setSliderProps={setSliderProps}
                    Values={Values} EndValues={EndValues}
                    obj={obj} opt={opt}
                    idx={idxMap} opt_key={optKey}
                  />

                ) : 
                !("step" in opt) ? (
                  <SliderNormal setEndSliderValue={setEndSliderValue} setSliderValue={setSliderValue}
                    Values={Values} EndValues={EndValues}
                    obj={obj} opt={opt}
                    idx={idxMap} optKey={optKey} 
                  />
                ) :
                (
                  <>
                    <OrderSwitch label={optKey} />
                    <Box h="5" />
                  </>
                )
              }
            </Fragment>
          );
        })}
      </Box>
    </Fragment>
  ));

  return (
    <Box>
      <Collapsible.Root justifyItems={"left"} alignContent={"left"}>

        <Collapsible.Trigger paddingY="3" display="flex" gap="2">

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

          <Grid templateColumns={{ base: "1fr", md: "repeat(2, 1fr)", lg: "repeat(3, 1fr)", }} gap={5} maxW="100%" mx="auto"
            alignItems="start" >
            {sliders}
          </Grid>

        </Collapsible.Content>

      </Collapsible.Root>

    </Box>
  )

}