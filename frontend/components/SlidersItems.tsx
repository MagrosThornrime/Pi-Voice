import { Box, Text, Collapsible, Grid } from "@chakra-ui/react";
import { useEffect, useState, Fragment } from "react";
import { LuChevronRight } from "react-icons/lu"
import { Opt, OptKey, Filter, filters, defaultOpts } from "../app/utils/tables"
import { buildInitialOptsState, buildInitialPropsState } from "../app/utils/state_utils";
import { LogSlider } from "./SliderLinLog";
import { OrderSwitch } from "./OrderSwitch";
import { SliderNormal } from "./SliderNormal";
import { useFiltersParams } from "@/app/utils/context_utils";


export type SliderProps = {
  bounds: number[];
  actValue: number;
}


type SlidersItemsProps = {
  neededItems: Filter[];
  attr: "filters" | "effects";
  idx: number;
};

export function SlidersItems({ neededItems, attr }: SlidersItemsProps) {
  const { paramsData, setParamsData } = useFiltersParams();

  const setSliderValue = (id: string, opt: string, newValue: number) => {
    setParamsData(prev =>
      prev.map(obj =>
        obj.id === id
          ? {
            ...obj,
            record: {
              ...obj.record,
              [opt as OptKey]: { ...obj.record[opt as OptKey], Val: newValue }
            }
          }
          : obj
      )
    );
  };

  const setEndSliderValue = (id: string, opt: string, newValue: number) => {
    setParamsData(prev =>
      prev.map(obj =>
        obj.id === id
          ? {
            ...obj,
            record: {
              ...obj.record,
              [opt as OptKey]: { ...obj.record[opt as OptKey], EndVal: newValue }
            }
          }
          : obj
      )
    );
  };

  const setSliderProps = (id: string, opt: string, newProps: SliderProps) => {
    setParamsData(prev =>
      prev.map(obj =>
        obj.id === id
          ? {
            ...obj,
            record: {
              ...obj.record,
              [opt as OptKey]: { ...obj.record[opt as OptKey], Props: newProps }
            }
          }
          : obj
      )
    );
  };



  const sliders = paramsData.map((obj, idx1) => (
    <Fragment key={obj.id}>
      <Box
        p={5}
        bg="grey"
        rounded="2xl"
        shadow="md"
      >
        <Text mb={2} fontWeight="medium" textAlign="center">
          {obj.value}
        </Text>

        {(Object.entries(defaultOpts) as [OptKey, Opt][]).map(([optKey, opt], idxMap) => {
          const stateKey = `${obj.value}.${optKey}`;
          //const value = paramsData[idx1].record[optKey].Val;

          return (
            <Fragment key={`${stateKey}${idxMap}`}>
              {
                "logScale" in opt && opt.logScale ? (

                  <LogSlider setSliderVal={setSliderValue} setSliderEndVal={setEndSliderValue}
                    setSliderProps={setSliderProps} opt={opt} paramsData={paramsData}
                    itemID={obj.id} optKey={optKey}
                  />

                ) : 
                !("step" in opt) ? (
                  <SliderNormal setEndSliderValue={setEndSliderValue} setSliderValue={setSliderValue}
                    opt={opt} paramsData={paramsData} itemID={obj.id}
                    optKey={optKey} 
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