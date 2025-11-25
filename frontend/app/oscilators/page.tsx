"use client";
import { Chart, useChart } from "@chakra-ui/charts";
import { Box,createListCollection,Flex,Group, Grid, Portal,Select,Stack,Text } from "@chakra-ui/react";
import React, { useState, useMemo} from "react";
import { CartesianGrid, Line, LineChart, Tooltip, XAxis, YAxis } from "recharts";


function get_example_data(n: number, domain: number[], func : (X:number) => number) {
  return Array.from({ length: n }, (_, i) => ({
    x: domain[0] + i * (domain[1] - domain[0])/n,
    y: func(domain[0] + i * (domain[1] - domain[0])/n),
  }));
}


const oscilatorTypes = createListCollection(
  {
  items: [
    { label: "Sine", value: "sine" },
    { label: "Square", value: "square" },
    { label: "Sawtooth", value: "sawtooth" },
    { label: "Triangle", value: "triangle" },
    { label: "Noise", value: "noise" },
    { label: "No oscilator", value: "empty" },
  ],
  }
)


function square_wave(x:number, interv: number){
  let position = x % interv;
  let eps = 0.01;

  if (position < eps){
    return 0;
  }

  if (Math.abs(position - interv/2) < eps){
    if (position > interv/2){
      return 1;
    }
    return 0;
  }

  if(position < interv/2){
    return 1;
  }

  return 0;
}


function triangle_wave(x:number, interv: number){
    let position = x % interv;
    console.log("position:", position)

    if(position <= interv/2){
      return position;
    }
    return interv/2 - (position - interv/2);
}


function sawtooth_func(x:number, interv: number){
    let position = x % interv;
    let eps = 0.01;
    if (x < eps){
      return 0.0;
    }
    if (position < eps){
      return 1/2 * interv;
    }
    if (interv - position < eps){
      return 0.0;
    }
    return 1/2 * position;

}


const oscillatorsFuncMapping: Record<string, (X:number) => number> = {
  sine: ((x) => Math.sin(3 * x)),
  square: ((x => square_wave(x, 2.0))),
  triangle: ((x) => triangle_wave(x, 2.0)),
  noise: ((x) => Math.sin(3 * x) + Math.random()/2),
  empty: (() => 0.0),
  sawtooth: ((x) => sawtooth_func(x, 2.0))
}

type FunctionChartProps = {
  func: (x: number) => number;
  domain?: [number, number];
  n?: number;
};


function FunctionChart({func, domain = [0, 10], n = 1000}: FunctionChartProps){

  const chart = useChart({
      data: get_example_data(n, domain, func),
      series: [{ name: "y", color: "teal.solid" }]
    })

    return (
      <Chart.Root width={400} height={300} chart={chart}>

        <LineChart data={chart.data}>

          <CartesianGrid vertical={false} />

          <XAxis dataKey="x"
            label={{ value: "X", position: "bottom" }}
            stroke={chart.color("border")}
            tickFormatter={(value) => `${Math.round(value * 100) / 100}`}
          />

          <YAxis dataKey="y"
            label={{ value: "Y", position: "left" }}
            stroke={chart.color("border")}
            tickFormatter={(value) => `${Math.round(value * 100) / 100}`}
          />

          <Tooltip
            animationDuration={100}
            cursor={false}
            content={({ active, payload, label }) => {
              if (active && payload && payload.length) {
                const x = Math.round(Number(label) * 100) / 100;
                const y = Math.round(payload[0].value * 100) / 100;

                return (
                  <Box bg="white" p={3} rounded="md" shadow="md" borderWidth={1}>
                    <Text fontSize="sm" color="gray.600">x: {x}</Text>
                    <Text fontSize="sm" color="gray.600">y: {y}</Text>
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
    )
}


const MemoFunctionChart = React.memo(FunctionChart);


export default function Page() {
  
  const [oscillators, setOscillator] = useState(["empty", "empty", "empty"])

  function changeOscillators(i:number, val:string){
    if (i < 0 || i > 2) throw new Error("index out of range (0..2)");
    setOscillator(prev => {
      const next = [...prev];
      next[i] = val;
      return next;
    });
  }

  return(
    <Box minH="100vh" bg="gray.50" p={10}>
      <Grid templateColumns={{
          base: "2fr",
          md: "repeat(3, 1fr)",
          lg: "repeat(3, 1fr)",
        }}
          gap={30}
          maxW="1400px"
          mx="auto"
          alignItems="center"
          justifyItems="center" >
        {
          oscillators.map((o, i) => (
            <Box key={i}>
              <MemoFunctionChart func = {oscillatorsFuncMapping[o]} />

              <Select.Root collection={oscilatorTypes} variant={"subtle"}
                onValueChange={(e) => {
                  changeOscillators(i, e.value[0]);
                  window.synthAPI.setOscillatorType(e.value[0], 0);
                }}>

                <Select.HiddenSelect />
                <Select.Label color={"black"}>{`Oscilator${i + 1}`}</Select.Label>

                <Select.Control>
                  <Select.Trigger>
                    <Select.ValueText color={"black"} placeholder="Select oscillator" />
                  </Select.Trigger>
                  <Select.IndicatorGroup>
                    <Select.Indicator />
                  </Select.IndicatorGroup>
                </Select.Control>
                <Portal>
                  <Select.Positioner>
                    <Select.Content>
                      {oscilatorTypes.items.map((oscilator) => (
                        <Select.Item color="black" item={oscilator} key={oscilator.value}>
                          {oscilator.label}
                          <Select.ItemIndicator />
                        </Select.Item>
                      ))}
                    </Select.Content>
                  </Select.Positioner>
                </Portal>
              </Select.Root>
            </Box>
          ))
        }  
      </Grid>
    </Box>
)}