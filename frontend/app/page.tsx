"use client";

import React, { useState } from "react";
import {
  Box,
  Heading,
  Text,
  Grid,
  Slider
} from "@chakra-ui/react";

import { Chart, useChart } from "@chakra-ui/charts"
import { CartesianGrid, Line, LineChart, Tooltip, XAxis, YAxis } from "recharts"

export interface Point {
  x: number
  y: number
}


// function get_example_data(n:number, eps:number){
//   var i:number; 
//   var res: Point[] = [];

//   for(i = 0; i < n; i++) {
//     const p: any = {
//       x: i * eps,
//       y: Math.sin(i * eps),
//     };
//     res.push(p as Point)
//   }
//   return res
// }

function get_example_data(n: number, eps: number, func : (X:number) => number) {
  return Array.from({ length: n }, (_, i) => ({
    x: i * eps,
    y: func(i * eps),
  }));
}


export default function Home() {

  const chart = useChart({
    data: get_example_data(100, 0.1, Math.sin),
    series: [{ name: "y", color: "teal.solid" }]
  })

  const data = get_example_data(100, 0.1, Math.sin);

  return (
    <Box minH="100vh" bg="gray.50" p={10}>

      <Heading textAlign="center" mb={10} color="teal.600">
        Audio Platform
      </Heading>

      <Box h="40" />

      <Chart.Root width={600} height={300} chart={chart} color="gray.600">
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
              if (
                active &&
                payload &&
                payload.length > 0 &&
                typeof label === "number" &&
                typeof payload[0].value === "number"
              ) {
              const x = Math.round(label * 100) / 100;
              const y = Math.round(payload[0].value * 100) / 100;

              return (
                <Box bg="white" p={3} rounded="md" shadow="md" borderWidth={1}>
                  <Text fontSize="sm">x: {x}</Text>
                  <Text fontSize="sm">y: {y}</Text>
                </Box>
                );
              }
              return null;
            } } 
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


      <Box h="80" />
      <Grid
        templateColumns={{
          base: "1fr",
          md: "repeat(2, 1fr)",
          lg: "repeat(3, 1fr)",
        }}
        gap={10}
        maxW="800px"
        mx="auto"
      >
        {["Volume", "Attack", "Sustain", "Decay", "Release", "Filter"].map((i) => (
          <Box key={i} p={5} bg="grey" rounded="2xl" shadow="md">
            <Text mb={2} fontWeight="medium" textAlign="center">
              {i}
            </Text>
            <Slider.Root defaultValue={[40]}>
              <Slider.Control>
                <Slider.Track>
                  <Slider.Range />
                </Slider.Track>
                <Slider.Thumbs />
              </Slider.Control>
            </Slider.Root>
          </Box>
        ))}
      </Grid>

    </Box>
  );
}


{/* <LineChart
      width={600}
      height={300}
      data={data}
      margin={{ top: 20, right: 20, left: 20, bottom: 20 }}
    >
      <CartesianGrid stroke="#ccc" />
      <XAxis dataKey="x" label={{ value: "x", position: "bottom" }} />
      <YAxis dataKey="y" label={{ value: "y", position: "left" }} />
      <Line type="monotone" dataKey="y" stroke="teal" dot={false} />
    </LineChart> */}


