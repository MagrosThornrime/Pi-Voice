import { Box, Text, Collapsible, Grid } from "@chakra-ui/react";
import { Fragment } from "react";
import { LuChevronRight } from "react-icons/lu"
import { Opt, OptKey, OptEffectKey, defaultOpts, defaultEffectOpts, EffectType } from "../app/utils/tables"
import { LogSlider } from "./SliderLinLog";
import { OrderSwitch } from "./OrderSwitch";
import { SliderNormal } from "./SliderNormal";
import { useFiltersParams } from "@/app/utils/context_utils";
import { OptParams } from "@/app/utils/context_utils";


export type SliderProps = {
  bounds: number[];
  actValue: number;
}

type SlidersItemsProps = {
  attr: "filters" | "effects";
};

export function SlidersItems({ attr }: SlidersItemsProps) {
  const { paramsData, setParamsData } = useFiltersParams();


  const setSliderValue = <P extends keyof OptParams>(
    id: string,
    opt: OptKey | OptEffectKey,
    property: P,
    newValue: OptParams[P]
  ) => {
    setParamsData(prev =>
      prev.map(item => {
        if (item.id !== id) return item

        if (item.params.group === "filters") {
          //filters
          const key = opt as OptKey
          return {
            ...item,
            params: {
              ...item.params,
              record: {
                ...item.params.record,
                [key]: {
                  ...item.params.record[key],
                  [property]: newValue
                }
              }
            }
          }
        }
        //effects
        const key = opt as OptEffectKey
        return {
          ...item,
          params: {
            ...item.params,
            record: {
              ...item.params.record,
              [key]: {
                ...item.params.record[key],
                [property]: newValue
              }
            }
          }
        }
      })
    )
  }



  const sliders = paramsData.map((obj, _) => {
    const group = obj.params.group;
    return (
    <Fragment key={obj.id}>
      <Box
        p={5}
        bg={(obj.params.group === "filters") ? "grey" : "green.600"}
        rounded="2xl"
        shadow="md"
      >
        <Text mb={2} fontWeight="medium" textAlign="center">
          {obj.params.value}
        </Text>
        {
        (Object.entries((group === "filters") ? defaultOpts : defaultEffectOpts[obj.params.value as EffectType]) as [OptKey | OptEffectKey, Opt][])
        .map(([optKey, opt], _) => {

          const stateKey = `${obj.id}.${optKey}`;
          return (
            <Fragment key={stateKey}>
              {
                "logScale" in opt && opt.logScale ? (

                  <LogSlider setSliderValue={setSliderValue} opt={opt} paramsData={paramsData}
                    itemID={obj.id} optKey={optKey}
                  />

                ) :
                  !("step" in opt && opt.range[1] - opt.range[0] === opt.step) ? (
                    <SliderNormal setSliderValue={setSliderValue}
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
    </Fragment>)

  });

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