"use client";

import { useState, useEffect, useMemo } from "react";
import { Box, Text, Grid, Slider} from "@chakra-ui/react";
import { Chart, useChart } from "@chakra-ui/charts"
import { CartesianGrid, Line, LineChart, Tooltip, XAxis, YAxis } from "recharts"
import { usePreset } from "@/components/ui/presetsProvider";
import { SliderTooltip } from "@/components/SliderTooltip";


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


function norm(param:number){
  return param/100;
}

function getFactor(v: number, minTime: number, maxTime: number){
  const sampleRate = 44100;
  const time = minTime * (maxTime / minTime) ** (v / 100);
  return 1 / (time * sampleRate);
}

export default function Home() {

  const {
      presetNr,
      presetProperties,
      setPresetProperties,
    } = usePreset();

  const [volumeValueVis, setVolumeValueVis] = useState(presetProperties.volume);
  const [attackValueVis, setAttackValueVis] = useState(presetProperties.attack);
  const [decayValueVis, setDecayValueVis] = useState(presetProperties.decay);
  const [sustainValueVis, setSustainValueVis] = useState(presetProperties.sustain);
  const [releaseValueVis, setReleaseValueVis] = useState(presetProperties.release);
  
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
    setEndValue: (v: number) => setPresetProperties(prev => ({...prev, volume: v})),
    onEnd: (v: number) => window.synthAPI.setAmplitude(v/100)
  },

  {
    label: "Attack",
    value: attackValueVis,
    setValue: setAttackValueVis,
    setEndValue: (v: number) => setPresetProperties(prev => ({...prev, attack: v})),
    onEnd: (v: number) => window.synthAPI.setAttack(getFactor(v, 0.001, 2.0))
  },

  {
    label: "Sustain",
    value: sustainValueVis,
    setValue: setSustainValueVis,
    setEndValue: (v: number) => setPresetProperties(prev => ({...prev, sustain: v})),
    onEnd: (v: number) => window.synthAPI.setSustain(v / 100)
  },

  {
    label: "Decay",
    value: decayValueVis,
    setValue: setDecayValueVis,
    setEndValue: (v: number) => setPresetProperties(prev => ({...prev, decay: v})),
    onEnd: (v: number) => window.synthAPI.setDecay(getFactor(v, 0.005, 4.0))
  },

  {
    label: "Release",
    value: releaseValueVis,
    setValue: setReleaseValueVis,
    setEndValue: (v: number) => setPresetProperties(prev => ({...prev, release: v})),
    onEnd: (v: number) => window.synthAPI.setRelease(getFactor(v, 0.01, 8.0))

  }
]

  useEffect(() => {
    setVolumeValueVis(presetProperties.volume);
    setAttackValueVis(presetProperties.attack);
    setDecayValueVis(presetProperties.decay);
    setSustainValueVis(presetProperties.sustain);
    setReleaseValueVis(presetProperties.release);
  }, [presetProperties.volume, presetProperties.attack, presetProperties.decay, presetProperties.sustain, presetProperties.release]);


  const charts = [chart_adsr];
  return (
    <Box minH="100vh" bg="gray.50" p={10} >


        <Box w = {{base: "100%", xl: "60%"}} bg = "gray" mx="auto" py = {{base:8, xl: 11}} rounded = "xl">
        <Box w="90%" bg = "white " mx="auto" rounded="xl" >
        <Chart.Root bg = "white" mx = "auto" py = {4} width = "80%" height={400} chart = {chart_adsr}>
          <LineChart data={chart_adsr.data}>

            <CartesianGrid vertical = {false} />

            <XAxis dataKey="x"
              stroke={chart_adsr.color("border")}
              tickFormatter={(value) => `${Math.round(value * 100)/100}`} 
            />

            <YAxis dataKey="y"
              stroke={chart_adsr.color("border")}
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
          chart_adsr.series.map((item) => (
            <Line key={item.name}
              isAnimationActive={false}
              dataKey={chart_adsr.key(item.name)}
              stroke={chart_adsr.color(item.color)}
              strokeWidth={2}
              dot={false} />
            )
          )
        }

        </LineChart>
      </Chart.Root>

      </Box>
      </Box>

      <Box h="10" />
      <Box justifyItems={"center"} bg = "gray.700" w={{base: "100%", xl: "60%"}} mx="auto" rounded="3xl" py = {8} px = {8}>
        <Grid
          templateColumns={{
            base: "1fr",
            md: "repeat(2, 1fr)",
            lg: "repeat(3, 1fr)",
          }}
          gap={10} maxW="1000px" mx="auto" >

            {
              adsrControls.map((ctrl) => (
                <Box key = {ctrl.label} p={5} bg="teal.500" rounded="2xl" shadow="md">
                  <Text mb={2} fontWeight="medium" textAlign="center">
                    {ctrl.label}
                  </Text>

                  <Slider.Root
                    value={[ctrl.value]}
                    onValueChange={(e) => ctrl.setValue(e.value[0])}
                    onValueChangeEnd={(e) => {
                      ctrl.setEndValue(e.value[0]);
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

                  <SliderTooltip Props={{bounds:[0, 100], actValue: ctrl.value}} />

                </Box>
                ) 
              )
            }

        </Grid>
      </Box>

    </Box>
  );
}


