"use client";

import { useState } from "react";

import {
  Box,
  Heading,
  Text,
  Grid,
  Slider,
  Stack,
  Button,
  List,
  ListItem,
} from "@chakra-ui/react";

declare global {
    interface Window {
        synthAPI: {
            ports: () => Promise<string[]>;
            openPort: (port: number) => Promise<void>;
            start: () => Promise<void>;
            stop: () => Promise<void>;
            cleanup: () => Promise<void>;
            startRecording: () => Promise<void>;
            stopRecording: () => Promise<void>;

            // pipelineAPI
            pipelineAddFilter: (filter: number, idx: number) => Promise<void>;
            pipelineRemove: (idx: number) => Promise<void>;
            pipelineMove: (current: number, target: number) => Promise<void>;
            pipelineSwap: (i1: number, i2: number) => Promise<void>;
            pipelineSetFilterParam: (idx: number, param: number, value: number) => Promise<void>;
            pipelineGetFilterParam: (idx: number, param: number) => Promise<number>;
            pipelineLength: () => Promise<number>;
        };
    }
}

export default function PlayPage() {
    const [status, setStatus] = useState<string>("idle");
    const [midiPorts, setMidiPorts] = useState<string[]>([]);
    const [selectedPort, setSelectedPort] = useState<number | null>(null);
    const [buttonText, setButtonText] = useState<string>("Start");

    const listPorts = async () => {
        setStatus("Fetching MIDI ports...");
        const ports = await window.synthAPI.ports();
        setMidiPorts(ports);
        setStatus(ports.length ? `Found ${ports.length} MIDI port(s)` : "No MIDI ports found");
    };

    const openPort = async (port: number) => {
        setSelectedPort(port);
        setStatus(`Opening port ${port}...`);
        await window.synthAPI.openPort(port);
        setStatus(`Port ${port} opened`);
    };

    return (
        <Box minH="100vh" bg="gray.200" p={10}>

            <Heading size="2xl" textAlign="center" mb={10} color="teal.600">
            🎛️ Simple Synth Controller
            </Heading>

            <Text color="black"> Status: {status} </Text>
            <Box h="5" />
            <Box>
                <Button onClick = {listPorts}> List MIDI Ports </Button>
                <List.Root>
                {
                        midiPorts.map((port, i) => (
                        <ListItem
                            key={port || i}
                            display="flex"
                            alignItems="center"
                            justifyContent="space-between"
                            p={3}
                            borderWidth="1px"
                            borderRadius="md"
                            _hover={{ bg: "gray.50" }}
                            >
                            <Box>
                                <Text fontWeight="medium">{port}</Text>
                                {
                                    port && (
                                    <Text fontSize="sm" color="gray.500">
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
                    )
                }
                </List.Root>
                <Button
                    bg={buttonText === "Start" ? "green.400" : "red.400"}
                    onClick={async () => {
                        try {
                            if (buttonText === "Start") {
                                setButtonText("Stop");
                                setStatus("Recording...");
                                await window.synthAPI.startRecording();
                            } else {
                                setButtonText("Start");
                                setStatus("Recording stopped.");
                                await window.synthAPI.stopRecording();
                            }
                        } catch (err) {
                            console.error(err);
                            setStatus("Error controlling recording.");
                        }
                    }}
                >
                    {buttonText} Recording
                </Button>

            </Box>
        </Box>
    );
}
