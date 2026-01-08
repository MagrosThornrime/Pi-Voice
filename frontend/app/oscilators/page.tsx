"use client";
import { Chart, useChart } from "@chakra-ui/charts";
import { Box, createListCollection, Grid, Portal, Select, Text, ListCollection, Checkbox, Separator } from "@chakra-ui/react";
import { useState, useEffect, memo, Fragment } from "react";
import { CartesianGrid, Line, LineChart, Tooltip, XAxis, YAxis } from "recharts";
import { usePreset } from "@/components/ui/presetsProvider";
import { getComputedOscillator } from "../utils/integration_utils";


type OscillatorItem = {
  value: string;
  label: string;
};


export type Point = {
  x: number;
  y: number;
}


function get_example_data(
  n: number,
  domain: number[],
  func: (x: number) => number
): Point[] {
  return Array.from({ length: n }, (_, i): Point => {
    const x = domain[0] + (i * (domain[1] - domain[0])) / n;
    return {
      x,
      y: func(x),
    };
  });
}


function square_wave(x: number, interv: number) {
  let position = x % interv;
  let eps = 0.01;

  if (position < eps) {
    return 0;
  }

  if (Math.abs(position - interv / 2) < eps) {
    if (position > interv / 2) {
      return 1;
    }
    return 0;
  }

  if (position < interv / 2) {
    return 1;
  }

  return 0;
}


function triangle_wave(x: number, interv: number) {
  let position = x % interv;
  console.log("position:", position)

  if (position <= interv / 2) {
    return position;
  }
  return interv / 2 - (position - interv / 2);
}


function sawtooth_func(x: number, interv: number) {
  let position = x % interv;
  let eps = 0.01;
  if (x < eps) {
    return 0.0;
  }
  if (position < eps) {
    return 1 / 2 * interv;
  }
  if (interv - position < eps) {
    return 0.0;
  }
  return 1 / 2 * position;

}


async function getOscPlotData(oscName: string) {
  let data: number[];
  try {
      data = await window.synthAPI.getOscillatorPlot(oscName);
  } catch { data = [] }
  const dataPoints = data.map((y, x): Point => {
    return { x, y };
  })
  return dataPoints;
}


const oscillatorsFuncMapping: Record<string, (X: number) => number> = {
  sine: ((x) => Math.sin(3 * x)),
  square: ((x => square_wave(x, 2.0))),
  triangle: ((x) => triangle_wave(x, 2.0)),
  empty: (() => 0.0),
  sawtooth: ((x) => sawtooth_func(x, 2.0))
}


type FunctionProps = {
  func: (x: number) => number;
  domain: [number, number];
  n: number;
}


type DataProps = {
  points: Point[];
}


type FunctionModeProps = {
  inputType: "function";
  givenFunc: FunctionProps;
};

type DataModeProps = {
  inputType: "data";
  givenData: DataProps;
};

type FunctionChartProps = FunctionModeProps | DataModeProps;


function FunctionChart(props: FunctionChartProps) {
  const chart = useChart({
    data: props.inputType === "function" ? get_example_data(props.givenFunc.n, props.givenFunc.domain, props.givenFunc.func) : props.givenData.points,
    series: [{ name: "y", color: "teal.500" }]
  })

  return (
    <Box bg = "white" rounded = "2xl">
    <Chart.Root width="100%" mx = "auto" height={{xl:400,base:290}} py = {4} pr = {4} chart={chart}>

      <LineChart data={chart.data}>

        <CartesianGrid vertical={false} />

        <XAxis dataKey="x"
          stroke={chart.color("border")}
          tickFormatter={(value) => `${Math.round(value * 100) / 100}`}
        />

        <YAxis dataKey="y"
          stroke={chart.color("border")}
          tickFormatter={(value) => `${Math.round(value * 100) / 100}`}
          tick={{ fill: "white" }}
        />

        <Tooltip
          animationDuration={100}
          cursor={false}
          content={({ active, payload, label }) => {
            if (active && payload && payload.length) {
              const x = Math.round(Number(label) * 100) / 100;
              const y = Math.round(payload[0].value * 100) / 100;

              return (
                <Box p={3} rounded="md" shadow="md" borderWidth={1}>
                  <Text color = "white" fontSize="sm" >x: {x} </Text>
                  <Text color = "white" fontSize="sm" >y: {y} </Text>
                </Box>
              );
            }
          }}
        />

        {
          chart.series.map((item) => (
            <Line key={item.name}
              isAnimationActive={false}
              dataKey={chart.key(item.name)}
              stroke={chart.color(item.color)}
              strokeWidth={2}
              dot={false}
            />
          )
          )
        }

      </LineChart>

    </Chart.Root>
    </Box>
  )
}


const MemoFunctionChart = memo(FunctionChart);
function getOscillatorFunction(name: string) {
  return oscillatorsFuncMapping[name] ?? oscillatorsFuncMapping["empty"]
}

const oscKeys = ["oscilator1", "oscilator2", "oscilator3"] as const;

export default function Page() {

  const {
    presetNr,
    presetProperties,
    setPresetProperties,
  } = usePreset();

  const [oscillators, setOscillator] = useState([presetProperties.oscilator1, presetProperties.oscilator2, presetProperties.oscilator3])
  const [points1, setPoints1] = useState<Point[]>([]);
  const [points2, setPoints2] = useState<Point[]>([]);
  const [points3, setPoints3] = useState<Point[]>([]);

  function changeOscillators(i: number, val: string) {
    if (i < 0 || i > 2) throw new Error("index out of range (0..2)");
    setPresetProperties(prev => ({ ...prev, [oscKeys[i]]: val }));
    setOscillator(prev => {
      const next = [...prev];
      next[i] = val;
      return next;
    });
  }

  const [oscillatorTypes, setOscillatorTypes] = useState<ListCollection<OscillatorItem>>(
    createListCollection<OscillatorItem>({ items: [] })
  );

  useEffect(() => {
    const load = async () => {
      const names = await window.synthAPI.getOscillatorNames();
      const formatted = names.map((name) => ({
        value: name,
        label: name,
      }));
      setOscillatorTypes(
        createListCollection({ items: formatted })
      );
    };

    load();
  }, []);

  useEffect(() => {
    setOscillator([presetProperties.oscilator1, presetProperties.oscilator2, presetProperties.oscilator3]);
  }, [presetProperties.oscilator1, presetProperties.oscilator2, presetProperties.oscilator3]);

  useEffect(() => {
    const loadPoints = async () => {
      const dataPoints = await getComputedOscillator(presetProperties.oscilator1);
      // can be replaced with getOscPlotData
      setPoints1(dataPoints);
    }
    loadPoints();
  }, [presetProperties.oscilator1]);

  useEffect(() => {
    const loadPoints = async () => {
      const dataPoints = await getComputedOscillator(presetProperties.oscilator2);
      // can be replaced with getOscPlotData
      setPoints2(dataPoints);
    }
    loadPoints();

  }, [presetProperties.oscilator2]);

  useEffect(() => {
    const loadPoints = async () => {
      const dataPoints = await getComputedOscillator(presetProperties.oscilator3);
      // can be replaced with getOscPlotData
      setPoints3(dataPoints);
    }
    loadPoints();

  }, [presetProperties.oscilator3]);

  const getPoints = (i: number): Point[] => {
    if (i < 0 || i > 2) { throw new Error("index out of range (0..2)"); }
    else if (i == 0) { return points1; }
    else if (i == 1) { return points2; }
    else { return points3 };
  }

  return (
    <Box minH="100vh"  p={10}>
      <Grid templateColumns={{
        base: "1fr",
        md: "repeat(2, 1fr)",
        lg: "repeat(2, 1fr)",
        xl: "repeat(3, 1fr)"
      }}
        gap={30}
        mx="auto"
        alignItems="center"
        justifyItems="center" >
        {
          oscillators.map((o, i) => (
            <Box key={i}>
              <MemoFunctionChart inputType={"data"} givenData={{ points: getPoints(i) }} />
              <Box h={{xl:"10",base:"3"}}/>

              <Box mt={4} p={6}  bg="teal.500"rounded="2xl" w="100%">

                <Select.Root colorPalette = "white" size={{lg:"sm", xl:"lg",}} collection={oscillatorTypes} variant={"subtle"}
                  positioning={{ sameWidth: true, placement: "bottom", flip: false }}
                  onValueChange={(e) => {
                    changeOscillators(i, e.value[0]);
                    try {
                      window.synthAPI.setOscillatorType(e.value[0], i);
                    } catch { }
                  }}>

                  <Select.HiddenSelect />
                  <Select.Label fontSize={{lg:"md", xl:"xl"}} fontWeight={"semibold"} color={"white"}>
                    {`Oscillator${i + 1}`}
                  </Select.Label>
                  <Box h="5" display={{ lg: "none", xl: "block" }}/>

                  <Select.Control>
                    <Select.Trigger>
                      <Select.ValueText fontSize={{lg:"md", xl:"xl"}} color={"blue.800"} placeholder={oscillators[i]} />
                    </Select.Trigger>

                    <Select.IndicatorGroup>
                      <Select.Indicator />
                    </Select.IndicatorGroup>
                  </Select.Control>

                  <Portal>
                    <Select.Positioner>
                      <Select.Content colorPalette={"teal"}>

                        {
                          oscillatorTypes.items.map((oscilator) => (
                          <Fragment key={`${oscilator.value}${i}`}>
                              <Select.Item fontSize={"xl"} item={oscilator} key={oscilator.value}
                              _hover={{ bg: "teal.100" }}
                              // _selected={{ bg: "blue.200" }}
                              >
                                <Text color = "teal.700"> {oscilator.label} </Text>
                                <Checkbox.Root size={"lg"} checked={oscilator.value === oscillators[i]} pointerEvents="none" mr={2} >
                                  <Checkbox.HiddenInput />
                                  <Checkbox.Control  border="none"/>
                                </Checkbox.Root>
                              </Select.Item>
                              <Separator flex="1" />
                          </Fragment>
                          ))
                        }

                      </Select.Content>
                    </Select.Positioner>
                  </Portal>
                </Select.Root>
              </Box>
            </Box>

          ))
        }
      </Grid>
    </Box>
  )
}
