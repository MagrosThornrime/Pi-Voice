"use client";

import { Fragment, useState } from "react";

import {
    Box,
    Heading,
    Text,
    Button,
    List,
    ListItem, Flex, Stack,
    Alert, CloseButton, Highlight
} from "@chakra-ui/react";

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
                    Simple Synth Controller
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