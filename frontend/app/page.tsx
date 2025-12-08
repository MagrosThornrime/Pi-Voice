"use client";

import React, { useState, useEffect, useMemo, useRef } from "react";
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
import { usePreset } from "@/components/ui/presetsProvider";


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

  const {
      presetNr,
      volumeValue,
      setVolumeValue,
      attackValue,
      setAttackValue,
      decayValue,
      setDecayValue,
      sustainValue,
      setSustainValue,
      releaseValue,
      setReleaseValue,
      savePreset,
    } = usePreset();

  const [volumeValueVis, setVolumeValueVis] = useState(volumeValue);
  const [attackValueVis, setAttackValueVis] = useState(attackValue);
  const [decayValueVis, setDecayValueVis] = useState(decayValue);
  const [sustainValueVis, setSustainValueVis] = useState(sustainValue);
  const [releaseValueVis, setReleaseValueVis] = useState(releaseValue);
  
  const data_adsr = useMemo(() => {
    return get_adsr_curve(norm(attackValueVis), norm(decayValueVis), norm(sustainValueVis), norm(releaseValueVis), 1.0, 100);
  }, [attackValueVis, decayValueVis, sustainValueVis, releaseValueVis]);

  const chart_adsr = useChart({
    data: data_adsr,
    series: [{ name: "y", color: "teal.solid" }]
  })

  const adsrControls = [
  {
    label: "Volume",
    value: volumeValueVis,
    setValue: setVolumeValueVis,
    setEndValue: setVolumeValue,
    onEnd: (v: number) => window.synthAPI.setAmplitude(v/100)
  },

  {
    label: "Attack",
    value: attackValueVis,
    setValue: setAttackValueVis,
    setEndValue: setAttackValue,
    onEnd: (v: number) => window.synthAPI.setAttack(10 ** (-v / 10))
  },

  {
    label: "Sustain",
    value: sustainValueVis,
    setValue: setSustainValueVis,
    setEndValue: setSustainValue,
    onEnd: (v: number) => window.synthAPI.setSustain(v / 100)
  },

  {
    label: "Decay",
    value: decayValueVis,
    setValue: setDecayValueVis,
    setEndValue: setDecayValue,
    onEnd: (v: number) => window.synthAPI.setDecay(10 ** (-v / 10))
  },

  {
    label: "Release",
    value: releaseValueVis,
    setValue: setReleaseValueVis,
    setEndValue: setReleaseValue,
    onEnd: (v: number) => window.synthAPI.setRelease(10 ** (-v / 10))
  }
]
  const isFirstRender = useRef(0);

  useEffect(() => {
    setVolumeValueVis(volumeValue);
    setAttackValueVis(attackValue);
    setDecayValueVis(decayValue);
    setSustainValueVis(sustainValue);
    setReleaseValueVis(releaseValue);
    if (isFirstRender.current<2){
      isFirstRender.current = isFirstRender.current+1;
    }else{
      savePreset(String(presetNr));
      console.log("saved adsr");
    }
  }, [volumeValue, attackValue, decayValue, sustainValue, releaseValue]);


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
        gap={10} maxW="800px" mx="auto">

          {
            adsrControls.map((ctrl) => (
              <Box key = {ctrl.label} p={5} bg="grey" rounded="2xl" shadow="md">
                <Text mb={2} fontWeight="medium" textAlign="center">
                  {ctrl.label}
                </Text>

                <Slider.Root
                  value={ctrl.value}
                  onValueChange={(e) => ctrl.setValue(e.value)}
                  onValueChangeEnd={(e) => {
                    ctrl.setEndValue(e.value);
                    ctrl.onEnd(e.value[0]);
                    console.log(presetNr);
                  }} >

                  <Slider.Control>
                    <Slider.Track>
                      <Slider.Range />
                    </Slider.Track>
                    <Slider.Thumbs />
                  </Slider.Control>
                </Slider.Root>

                <Stack mt="3" gap="1">
                  <Text>
                    Wartość: <b>{Math.round(ctrl.value * 100) / 100}</b>
                  </Text>
                </Stack>

              </Box>
              ) 
            )
          }

      </Grid>

    </Box>
  );
}


