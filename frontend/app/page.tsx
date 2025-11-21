"use client";

import React, { useState, useEffect, useMemo } from "react";
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

function norm(param:number[]){
  return param[0]/100;
}


export default function Home() {

  const [volumeValue, setVolumeValue] = useState([10])
  const [endVolumeValue, setEndVolumeValue] = useState([40])

  const [attackValue, setAttackValue] = useState([10])
  const [endAttackValue, setEndAttackValue] = useState([40])

  const [decayValue, setDecayValue] = useState([20])
  const [endDecayValue, setEndDecayValue] = useState([40])

  const [sustainValue, setSustainValue] = useState([20])
  const [endSustainValue, setEndSustainValue] = useState([40])


  const [releaseValue, setReleaseValue] = useState([40])
  const [endReleaseValue, setEndReleaseValue] = useState([40])


  // type adsrParams = {
  //   attackValue: number;
  //   decayValue: number;
  //   sustainValue: number;
  //   releaseValue: number
  // };

  // type adsrEndParams = {
  //   endAttackValue: number;
  //   endDecayValue: number;
  //   endSustainValue: number;
  //   endReleaseValue: number
  // }

  // const [adsrProps, setAdsrProps] = useState<adsrParams>(
  //   {
  //     attackValue: 10,
  //     decayValue: 20,
  //     sustainValue: 30,
  //     releaseValue: 40
  //   }
  // )

  // const [endAdsrProps, setEndAdsrProps] = useState<adsrEndParams>(
  //   {
  //     endAttackValue: 10,
  //     endDecayValue: 20,
  //     endSustainValue: 30,
  //     endReleaseValue: 40
  //   }
  // )

  // const data_adsr = useMemo(() => {
  //   return get_adsr_curve(adsrProps.attackValue/100, adsrProps.decayValue/100, adsrProps.sustainValue/100, adsrProps.releaseValue/100, 1.0, 100);
  // }, [adsrProps]);

  
  const data_adsr = useMemo(() => {
    return get_adsr_curve(norm(attackValue), norm(decayValue), norm(sustainValue), norm(releaseValue), 1.0, 100);
  }, [attackValue, decayValue, sustainValue, releaseValue]);

  const chart_adsr = useChart({
    data: data_adsr,
    series: [{ name: "y", color: "teal.solid" }]
  })


  const charts = [chart_adsr];

  return (
    <Box minH="100vh" bg="gray.50" p={10}>

      <Heading size="3xl" textAlign="center" mb={10} color="teal.600">
        Audio Platform
      </Heading>

      <Box h="10" />

      <Grid
        templateColumns={{
          base: "1fr",
          md: "repeat(1, 1fr)",
          lg: "repeat(1, 1fr)",
        }}
        
          gap={8}
          maxW="1200px"
          mx="auto"
          alignItems="center"
          justifyItems="center"
      >
        {
          charts.map((chart) => (

          <Chart.Root key={chart.id} width = "100%" height={400} chart = {chart}>
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
                }
                } 
              />

          {
            chart.series.map((item) => (
              <Line key={item.name}
                isAnimationActive={false}
                dataKey={chart.key(item.name)}
                stroke={chart.color(item.color)}
                strokeWidth={2}
                dot={false} />
              )
            )
          }

          </LineChart>
        </Chart.Root>
        )
      )
      }

      </Grid>

      <Box h="10" />
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
              }
            }
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


