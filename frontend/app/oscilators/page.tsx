"use client";

import { Chart, useChart } from "@chakra-ui/charts";
import { Box,createListCollection,Flex,Group,Portal,Select,Stack,Text } from "@chakra-ui/react";
import { useState } from "react";
import { CartesianGrid, Line, LineChart, Tooltip, XAxis, YAxis } from "recharts";

function get_example_data(n: number, eps: number, func : (X:number) => number) {
  return Array.from({ length: n }, (_, i) => ({
    x: i * eps,
    y: func(i * eps),
  }));
}

const oscilatorTypes = createListCollection({
  items: [
    { label: "Sine", value: "sine" },
    { label: "Square", value: "square" },
    { label: "Sawtooth", value: "sawtooth" },
    { label: "Triangle", value: "triangle" },
    { label: "Noise", value: "noise" },
    { label: "No oscilator", value: "empty" },
  ],
})

export default function Page() {
  const [oscilator1, setOscilator1] = useState<string>("empty")
  const [oscilator2, setOscilator2] = useState<string>("empty")
  const chart = useChart({
      data: get_example_data(100, 0.1, Math.sin),
      series: [{ name: "y", color: "teal.solid" }]
    })
  
    const data = get_example_data(100, 0.1, Math.sin);
  return(
    <Box minH="100vh" bg="gray.50" p={10}>
      <Group>
        <Stack>
          <Chart.Root width={600} height={300} chart={chart}>
            <LineChart data={chart.data}>

              <CartesianGrid vertical={false} />

              <XAxis dataKey="x"
                label={{ value: "X", position: "bottom" }}
                stroke={chart.color("border")}
                tickFormatter={(value) => `${Math.round(value * 100)/100}`} 
              />

              <YAxis dataKey="y"
                label={{ value: "Y", position: "left" }}
                stroke={chart.color("border")}
                tickFormatter={(value) => `${Math.round(value * 100)/100}`} 
              />

              <Tooltip
                animationDuration={100}
                cursor={false}
                content={({ active, payload, label }) => {
                  if (active && payload && payload.length) 
                  {
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
          <Select.Root collection={oscilatorTypes} variant={"subtle"} onValueChange={(e) => {
            setOscilator1(e.value[0]);
            window.synthAPI.setOscillatorType(e.value[0],0);
          }}>
            <Select.HiddenSelect />
            <Select.Label color={"black"}>Oscilator1</Select.Label>
            <Select.Control>
              <Select.Trigger>
                <Select.ValueText placeholder="Select oscilator" />
              </Select.Trigger>
              <Select.IndicatorGroup>
                <Select.Indicator />
              </Select.IndicatorGroup>
            </Select.Control>
            <Portal>
              <Select.Positioner>
                <Select.Content>
                  {oscilatorTypes.items.map((oscilator) => (
                    <Select.Item item={oscilator} key={oscilator.value}>
                      {oscilator.label}
                      <Select.ItemIndicator />
                    </Select.Item>
                  ))}
                </Select.Content>
              </Select.Positioner>
            </Portal>
          </Select.Root>
        </Stack>
        <Stack>
          <Chart.Root width={600} height={300} chart={chart}>
            <LineChart data={chart.data}>

              <CartesianGrid vertical={false} />

              <XAxis dataKey="x"
                label={{ value: "X", position: "bottom" }}
                stroke={chart.color("border")}
                tickFormatter={(value) => `${Math.round(value * 100)/100}`} 
              />

              <YAxis dataKey="y"
                label={{ value: "Y", position: "left" }}
                stroke={chart.color("border")}
                tickFormatter={(value) => `${Math.round(value * 100)/100}`} 
              />

              <Tooltip
                animationDuration={100}
                cursor={false}
                content={({ active, payload, label }) => {
                  if (active && payload && payload.length) 
                  {
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
          <Select.Root collection={oscilatorTypes} variant={"subtle"} onValueChange={(e) => {
            setOscilator2(e.value[0]);
            window.synthAPI.setOscillatorType(e.value[0],1);
          }}>
            <Select.HiddenSelect />
            <Select.Label color={"black"}>Oscilator2</Select.Label>
            <Select.Control>
              <Select.Trigger>
                <Select.ValueText placeholder="Select oscilator" />
              </Select.Trigger>
              <Select.IndicatorGroup>
                <Select.Indicator />
              </Select.IndicatorGroup>
            </Select.Control>
            <Portal>
              <Select.Positioner>
                <Select.Content>
                  {oscilatorTypes.items.map((oscilator) => (
                    <Select.Item item={oscilator} key={oscilator.value}>
                      {oscilator.label}
                      <Select.ItemIndicator />
                    </Select.Item>
                  ))}
                </Select.Content>
              </Select.Positioner>
            </Portal>
          </Select.Root>
        </Stack>
      </Group>
    </Box>
)}