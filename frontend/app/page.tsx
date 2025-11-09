"use client";

import React, { useState } from "react";
import {
  Box,
  Heading,
  Text,
  Grid,
  Slider,
  Stack
} from "@chakra-ui/react";

import { Chart, useChart } from "@chakra-ui/charts"
import { CartesianGrid, Line, LineChart, Tooltip, XAxis, YAxis } from "recharts"

export interface Point {
  x: number
  y: number
}


function get_example_data(n: number, eps: number, func : (X:number) => number, shift: number = 0): Point[] {
  return Array.from({ length: n }, (_, i) => ({
    x: i * eps + shift,
    y: func(i * eps + shift),
  }));
}


function get_adsr_curve(attack: number, decay: number, sustain: number, release: number, totalTime: number, sampleRate: number): Point[] {
  
  const points: Point[] = [];

  // sustain value actually only represents how low should decay drop, release parameter regulates the length of sustain phase

  var T_attack = attack * totalTime;
  var T_decay = decay * totalTime;
  var T_release = release * totalTime;
  const used = T_attack + T_decay + T_release;
  
  if (used > totalTime){
    const scale = totalTime/used;
    T_attack *= scale;
    T_decay *= scale;
    T_release *= scale;
  }


  const attackEnd = T_attack;
  const decayEnd = attackEnd + T_decay;
  const releaseStart = totalTime - T_release;

  for (let t = 0; t <= totalTime; t += 1 / sampleRate) {
    let amplitude: number;
    if (t < attackEnd) {
      amplitude = t/T_attack;
    } else if (t < decayEnd) {
      amplitude = 1 - ((t - T_attack) / T_decay ) * (1 - sustain);
    } else if (t < releaseStart) {
      amplitude = sustain;
    } else {
      amplitude = sustain * (1 - (t - releaseStart) / T_release);
    }
    points.push({ x: t, y: amplitude });
  }
  return points;
}


export default function Home() {

  // const chart = useChart({
  //   data: get_example_data(100, 0.1, Math.sin),
  //   series: [{ name: "y", color: "teal.solid" }]
  // })

  // const data = get_example_data(100, 0.1, Math.sin);

  const [volumeValue, setVolumeValue] = useState([40])
  const [endVolumeValue, setEndVolumeValue] = useState([40])

  const [attackValue, setAttackValue] = useState([20])
  const [endAttackValue, setEndAttackValue] = useState([40])

  const [sustainValue, setSustainValue] = useState([20])
  const [endSustainValue, setEndSustainValue] = useState([40])

  const [decayValue, setDecayValue] = useState([10])
  const [endDecayValue, setEndDecayValue] = useState([40])

  const [releaseValue, setReleaseValue] = useState([20])
  const [endReleaseValue, setEndReleaseValue] = useState([40])

  const chart = useChart({
    data: get_adsr_curve(0.1, 0.2, 0.2, 0.4, 1.0, 100),
    series: [{ name: "y", color: "teal.solid" }]
  })

  return (
    <Box minH="100vh" bg="gray.50" p={10}>

      <Heading textAlign="center" mb={10} color="teal.600">
        Audio Platform
      </Heading>

      <Box h="40" />

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
          <Box p={5} bg="grey" rounded="2xl" shadow="md">
            <Text mb={2} fontWeight="medium" textAlign="center">
              Volume
            </Text>
            <Slider.Root
              value={volumeValue}
              onValueChange={(e) => setVolumeValue(e.value)}
              onValueChangeEnd={(e) => {
                setEndVolumeValue(e.value);
                window.synthAPI.setAmplitude(e.value[0] / 100.0);
              }}
            >
              <Slider.Control>
                <Slider.Track>
                  <Slider.Range />
                </Slider.Track>
                <Slider.Thumbs />
              </Slider.Control>
            </Slider.Root>
            <Stack mt="3" gap="1">
              <Text>
                Wartość: <b>{volumeValue}</b>
              </Text>
            </Stack>
          </Box>
          <Box p={5} bg="grey" rounded="2xl" shadow="md">
            <Text mb={2} fontWeight="medium" textAlign="center">
              Attack
            </Text>
            <Slider.Root
              value={attackValue}
              onValueChange={(e) => setAttackValue(e.value)}
              onValueChangeEnd={(e) => {
                setEndAttackValue(e.value);
                window.synthAPI.setAttack((10**(-e.value[0]/10)));
              }}
            >
              <Slider.Control>
                <Slider.Track>
                  <Slider.Range />
                </Slider.Track>
                <Slider.Thumbs />
              </Slider.Control>
            </Slider.Root>
            <Stack mt="3" gap="1">
              <Text>
                Wartość: <b>{attackValue}</b>
              </Text>
            </Stack>
          </Box>
          <Box p={5} bg="grey" rounded="2xl" shadow="md">
            <Text mb={2} fontWeight="medium" textAlign="center">
              Sustain
            </Text>
            <Slider.Root
              value={sustainValue}
              onValueChange={(e) => setSustainValue(e.value)}
              onValueChangeEnd={(e) => {
                setEndSustainValue(e.value);
                window.synthAPI.setSustain(e.value[0] / 100.0);
              }}
            >
              <Slider.Control>
                <Slider.Track>
                  <Slider.Range />
                </Slider.Track>
                <Slider.Thumbs />
              </Slider.Control>
            </Slider.Root>
            <Stack mt="3" gap="1">
              <Text>
                Wartość: <b>{sustainValue}</b>
              </Text>
            </Stack>
          </Box>
          <Box p={5} bg="grey" rounded="2xl" shadow="md">
            <Text mb={2} fontWeight="medium" textAlign="center">
              Decay
            </Text>
            <Slider.Root
              value={decayValue}
              onValueChange={(e) => setDecayValue(e.value)}
              onValueChangeEnd={(e) => {
                setEndDecayValue(e.value);
                window.synthAPI.setDecay(10**(-e.value[0]/10));
              }}
            >
              <Slider.Control>
                <Slider.Track>
                  <Slider.Range />
                </Slider.Track>
                <Slider.Thumbs />
              </Slider.Control>
            </Slider.Root>
            <Stack mt="3" gap="1">
              <Text>
                Wartość: <b>{decayValue}</b>
              </Text>
            </Stack>
          </Box>
          <Box p={5} bg="grey" rounded="2xl" shadow="md">
            <Text mb={2} fontWeight="medium" textAlign="center">
              Release
            </Text>
            <Slider.Root
              value={releaseValue}
              onValueChange={(e) => setReleaseValue(e.value)}
              onValueChangeEnd={(e) => {
                setEndReleaseValue(e.value)
                window.synthAPI.setRelease(10**(-e.value[0]/10));
              }}
            >
              <Slider.Control>
                <Slider.Track>
                  <Slider.Range />
                </Slider.Track>
                <Slider.Thumbs />
              </Slider.Control>
            </Slider.Root>
            <Stack mt="3" gap="1">
              <Text>
                Wartość: <b>{releaseValue}</b>
              </Text>
            </Stack>
          </Box>
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


