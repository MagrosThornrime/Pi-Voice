"use client";

import { Fragment, useState } from "react";
import { Preset, PresetFile } from "@/components/ui/presetsProvider";

import {
    Box,
    Heading,
    Text,
    Button,
    List,
    ListItem, Flex, Stack,
    Alert, CloseButton, Highlight
} from "@chakra-ui/react";
import { changeParams } from "../sequencer/actions";
import { EffectType, FilterType, OptEffectKey, OptKey } from "../utils/tables";

declare global {
    interface Window {
        synthAPI: {
            ports: () => Promise<string[]>;
            openPort: (port: number) => Promise<void>;

            // setters
            setOscillatorType: (type: string, index: number) => Promise<void>;
            setAttack: (v: number) => Promise<void>;
            setDecay: (v: number) => Promise<void>;
            setSustain: (v: number) => Promise<void>;
            setRelease: (v: number) => Promise<void>;
            startRecording: () => Promise<void>;
            stopRecording: () => Promise<void>;

            // pipelineAPI
            pipelineAddFilter: (filter: number, idx: number) => Promise<void>;
            pipelineAddEffect: (effect: number, idx: number) => Promise<void>;
            pipelineRemove: (idx: number) => Promise<void>;
            pipelineMove: (current: number, target: number) => Promise<void>;
            pipelineSwap: (i1: number, i2: number) => Promise<void>;
            pipelineSetFilterParam: (idx: number, param: number, value: number) => Promise<void>;
            pipelineSetEffectParam: (idx: number, param: number, value: number) => Promise<void>;
            pipelineGetFilterParam: (idx: number, param: number) => Promise<number>;
            pipelineGetEffectParam: (idx: number, param: number) => Promise<number>;
            pipelineLength: () => Promise<number>;
            pipelineSetAmplitude: (value: number) => Promise<void>;

            // sequencerAPI
            sequencerIsActive: () => Promise<boolean>;
            sequencerIsRecording: () => Promise<boolean>;
            sequencerActivate: () => Promise<void>;
            sequencerDeactivate: () => Promise<void>;
            sequencerStartRecording: (sampleRate: number, channels: number, seconds: number) => Promise<void>;
            sequencerStopRecording: () => Promise<void>;
            sequencerRemoveSample: (i: number) => Promise<void>;
            sequencerClear: () => Promise<void>;
            sequencerSampleLength: (i: number) => Promise<number>;
            sequencerLength: () => Promise<number>;
            sequencerMoveSample: (curr: number, val: number) => Promise<void>;
            sequencerSwapSamples: (i1: number, i2: number) => Promise<void>;
            sequencerAddSample: (name: string) => Promise<void>;

            // sample manager
            setSamplesPath: (path: string) => Promise<void>;
            getOscillatorNames: () => Promise<string[]>;

            // waveform preview
            getOscillatorPlot: (name: string) => Promise<number[]>;

        };
        presetsAPI: {
            read: () => Promise<PresetFile>;
            write: (data: PresetFile) => Promise<void>;
            saveOne: (name: string, preset: Preset) => Promise<void>;
        };
        slidersAPI: {
            read: (sliderType: "lin" | "log", itemType: "filters" | "effects", sliderVal: number,
                itemName: EffectType | FilterType, paramName:OptEffectKey | OptKey, change:boolean, bounds?: number[]) => Promise<changeParams | number>;
        }
    }
}

export default function PlayPage() {
    const [status, setStatus] = useState<string>(() => {
        try {
            return sessionStorage.getItem("recording") === "Stop"
                ? "Recording..."
                : "idle";
        } catch {
            return "idle";
        }
    });
    const [midiPorts, setMidiPorts] = useState<string[]>([]);
    const [selectedPort, setSelectedPort] = useState<number | null>(null);

    const listPorts = async () => {
        const ports = await window.synthAPI.ports();
        setMidiPorts(ports);
        setStatus(ports.length ? `Found ${ports.length} MIDI port(s)` : "No MIDI ports found");
    };

    const openPort = async (port: number) => {
        setSelectedPort(port);
        await window.synthAPI.openPort(port);
        setStatus(`Port ${port} opened`);
    };

    return (
        <Box minH="100vh" bg="gray.200" mx = "auto" w = "70%" rounded = "xl" p={10}>
            <Fragment>

                <Heading size="3xl" textAlign="center" mb={10} color="purple.600">
                    Settings
                </Heading>

                <Flex align="center" justify="center">
                    <Alert.Root bg = "blue.300" w = "90%">
                        <Alert.Indicator />
                        <Alert.Content>
                            <Alert.Title fontSize = "xl" fontWeight = "semibold" textAlign={"center"} >Instruction</Alert.Title>
                            <Box h = "2"/>
                            <Alert.Description textAlign={"center"}>

                                <Highlight query= {["Refresh MIDI Ports", "Clear Ports"]}
                                    styles={{ bg: "yellow.400", fontWeight: "semibold" }}>

                                    In order to get list of all available MIDI ports,
                                    click Refresh MIDI Ports first, you can also hide them using Clear Ports

                                </Highlight>

                            </Alert.Description>
                        </Alert.Content>
                        <CloseButton pos="relative" top="-2" insetEnd="-2" />
                    </Alert.Root>
                </Flex>

                <Box h="10" />

                <Flex align="center" justify="center">

                    <Box rounded="2xl" bg={(status === "idle") ? "gray.500" : "green.600"} textAlign="center" minH={50} w="50%"
                        display="flex"alignItems="center" justifyContent="center">

                        <Stack direction = "row">
                        <Text fontSize="lg" fontWeight = "semibold" color="white">Status: {status}</Text>
                        </Stack>
                    </Box>
                </Flex>

                <Box h="5" />

                <Box>
                    <Box display="flex" justifyContent="center">
                        <Stack direction="row" gap={5}>
                            <Button size={"2xl"} onClick={listPorts}> Refresh MIDI Ports </Button>
                            <Button bg="red.500" size={"2xl"} onClick={(e) => {
                                setMidiPorts([]);
                                setStatus("idle");
                            }
                            }>
                                Clear Ports
                            </Button>
                        </Stack>
                    </Box>
                    <Box h="5" />
                    <Flex align="center" justify="center">
                        <List.Root w = "40%">
                            {
                                midiPorts.map((port, i) => {
                                    console.log("PORT:", port, midiPorts.length);
                                    return (
                                        <ListItem
                                            key={port || i}
                                            display="flex"
                                            alignItems="center"
                                            p={3}
                                            borderWidth="1px"
                                            borderRadius="md"
                                            bg={"purple.400"}
                                            _hover={{ bg: "blue.600" }}
                                        >
                                            <Box flex="1">
                                                {
                                                    port && (

                                                        <Text fontSize="lg" textAlign={"center"} fontWeight={"semibold"} color="white">
                                                            {port}
                                                        </Text>

                                                    )
                                                }
                                            </Box>

                                            <Button colorScheme="blue" onClick={() => openPort(i)}>
                                                Open Port {i}
                                            </Button>
                                        </ListItem>
                                    )
                                }
                                )
                            }
                        </List.Root>
                    </Flex>
                    <Box h="5" />
                    <Box display="flex" justifyContent="center">
                        <Button size={"2xl"}
                            bg={(() =>{
                                try{
                                    return (sessionStorage.getItem("recording") ?? "Start") === "Start" ? "green.600" : "red.600"
                                }catch{
                                    return "green.600";
                                }
                            })()}

                            onClick={async () => {
                                try {
                                    if ((sessionStorage.getItem("recording") ?? "Start") == "Stop"){
                                        sessionStorage.setItem("recording","Start");
                                        setStatus("Recording stopped.");
                                        await window.synthAPI.stopRecording();
                                    } else{
                                        sessionStorage.setItem("recording","Stop");
                                        setStatus("Recording...");
                                        await window.synthAPI.startRecording();
                                    }
                                } catch (err) {
                                    console.error(err);
                                    setStatus("Error controlling recording.");
                                }
                            }}
                        >
                            {
                                (() => {
                                    try {
                                        return ((sessionStorage.getItem("recording") ?? "Start") + " Recording")
                                    } catch {
                                        return "Record to sequencer";
                                    }
                                })()
                            }
                        </Button>
                    </Box>
                </Box>
            </Fragment>
        </Box>
    );
}