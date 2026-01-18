"use client";

import { useEffect, useRef, useState } from "react";

import {
  Box,
  Stack,
  Button,
  Grid,
  GridItem, Text,
  Editable, IconButton
} from "@chakra-ui/react";

import { LuPencilLine } from "react-icons/lu"
import { MdDelete } from "react-icons/md";


function getRandomColor() {
    let colors = ["teal", "green", "blue"].flatMap(color =>
        [600, 700, 800, 900].map(shade => `${color}.${shade}`)
    );
    colors = colors.concat(["purple.600", "purple.700"]);
    colors.filter((item) => item !== "green.500")

    return colors[Math.floor(Math.random() * colors.length)];
}

export default function Page() {

    const [isPlaying, setIsPlaying] = useState<boolean>(() => {
        try {
            return sessionStorage.getItem("seq_playing") === "Stop";
        } catch {
            return false;
        }
    });

    const [isRecording, setIsRecording] = useState<boolean>(() => {
        try{
            return sessionStorage.getItem("seq_recording") === "Stop recording";
        } catch{
            return false;
        }
    })

    const [sounds, setSounds] = useState<string[]>([]);
    const [colorMap, setColorMap] = useState<Record<string, string>>({});
    const [dragIndex, setDragIndex] = useState<number | null>(null);

    function generate_name(){
        let last = 1;
        while (sounds.includes("Dzwiek" + last)) last++;
        return "Dzwiek" + last;
    }

    const isFirstRender = useRef(0);

    useEffect(() => {
        const saved = sessionStorage.getItem("sounds");
        if (saved){
            setSounds(JSON.parse(saved));
            const saved2 = sessionStorage.getItem("colorMap");
            if (saved2) setColorMap(JSON.parse(saved2));
        }
    }, []);

    useEffect(() => {
        if (isFirstRender.current<1){
            isFirstRender.current = isFirstRender.current+1;
        }else{
            sessionStorage.setItem("sounds", JSON.stringify(sounds));
        }
        setColorMap(prev => {
            const copy: Record<string, string> = { ...prev };
            sounds.forEach(s => {
            if (!copy[s]) {
                copy[s] = getRandomColor();
            }
            });
            return copy;
        });
    }, [sounds]);

    useEffect(() => {
        if (isFirstRender.current >= 1){
            sessionStorage.setItem("colorMap", JSON.stringify(colorMap));
        }
    }, [colorMap]);

    const handleDragStart = (index: number) => {
        setDragIndex(index);
    };

    const handleDragOver = (e: React.DragEvent<HTMLDivElement>) => {
        e.preventDefault();
    };
    
    const handleDrop = async (index: number) => {
        if (dragIndex === null || dragIndex === index){setDragIndex(null); return;}

        const newArr = [...sounds];

        const [movedItem] = newArr.splice(dragIndex, 1);
        newArr.splice(index, 0, movedItem);

        setSounds(newArr);
        setDragIndex(null);

        await window.synthAPI.sequencerMoveSample(dragIndex,index);
    };

    return (
        <Box minH="100vh" bg="teal.100" p={10}>
            <Stack>
                <Stack direction={"row"} justifyContent={"center"} gap={10}>
                <Button 
                    w = {300}
                    disabled={isPlaying}
                    bg={(() => {
                        try {
                            return (sessionStorage.getItem("seq_recording") === "Stop recording") ? "red.600" : "green.600";
                        } catch {
                            return "green.600";
                        }
                    })()}

                    onClick={async () => {
                        try {
                            if (sessionStorage.getItem("seq_recording")=="Stop recording"){
                                setIsRecording(false);
                                sessionStorage.setItem("seq_recording","Record to sequencer");
                                await window.synthAPI.sequencerStopRecording();
                                setSounds(prev => [...prev, generate_name()]);
                            }else{
                                setIsRecording(true);
                                sessionStorage.setItem("seq_recording","Stop recording");
                                await window.synthAPI.sequencerStartRecording(44100,2,40);
                            }
                        } catch (err) {
                            console.error(err);
                        }
                    }}
                >
                        <Text fontWeight="bold" fontSize={"lg"} color = "white">
                            {
                                (() => {
                                    try {
                                        return sessionStorage.getItem("seq_recording") ?? "Record to sequencer";
                                    } catch {
                                        return "Record to sequencer";
                                    }
                                })()
                            }
                        </Text>
                </Button>
                <Button
                    w = {300}
                    disabled={isRecording||sounds.length==0}
                    bg={(() => {
                        try {
                            return (sessionStorage.getItem("seq_playing") === "Stop") ?  "red.600" : "green.600";
                        } catch { return "green.600";
                            }
                    })()}

                    onClick={async () => {
                        try {
                            if (sessionStorage.getItem("seq_playing") === "Stop") {
                                setIsPlaying(false);
                                sessionStorage.setItem("seq_playing","Play");
                                await window.synthAPI.sequencerDeactivate();                                
                            } else {
                                setIsPlaying(true);
                                sessionStorage.setItem("seq_playing","Stop");
                                await window.synthAPI.sequencerActivate();
                            }
                            console.log(sessionStorage.getItem("seq_playing"));
                        } catch (err) {
                            console.error(err);
                        }
                    }}
                >
                        <Text fontWeight="bold" fontSize={"lg"} color = "white">
                            {
                                (() => {
                                    try {
                                        return sessionStorage.getItem("seq_playing") ?? "Play";
                                    } catch {
                                        return "Play";
                                    }
                                })()
                            }
                        </Text>
                    
                </Button>
                </Stack>
                <Box h="20"/>
                <Grid
                    w="100%" bg = "blue.200" p = {5} rounded = "xl"
                    templateColumns={{lg:"repeat(4, 1fr)", xl:"repeat(8, 1fr)"}}
                    gap={4}
                    gapY={4}
                >
                    {
                        sounds.map((item, index) => (
                             <GridItem colSpan={1} key={item}>
                                <Box
                                    minHeight="60px" minWidth="13%" shadow="md" p={2}
                                    display="flex" alignItems="center"
                                    rounded="xl"
                                    justifyContent={{lg:"flex-start", xl:"flex-start"}}
                                    position="relative"
                                    bg={colorMap[item]}
                                    draggable={!isPlaying}
                                    onDragStart={() => {
                                        if(isPlaying)return;
                                        handleDragStart(index);
                                    }}
                                    onDragOver={(e) => {
                                        if(isPlaying)return;
                                        handleDragOver(e);
                                    }}
                                    onDrop={() => {
                                        if(isPlaying)return;
                                        handleDrop(index);
                                    }}
                                    cursor={!isPlaying ? "grab" : "not-allowed"}
                                    opacity={dragIndex === index ? 0.4 : 1}
                                >
                                    <Button size={{lg:"sm", xl:"lg"}}
                                        p={1}
                                        minW={0}
                                        bg="transparent"
                                        position="absolute"
                                        left="8px"
                                        color = "teal.200"
                                        _hover={{ bg: "red.600" }}
                                        _active={{ bg: "red.700" }}
                                        disabled={isPlaying}
                                        onClick={() => {
                                            setSounds(prev => prev.filter(s => s !== item));
                                            window.synthAPI.sequencerRemoveSample(index);
                                        }
                                    }>
                                        <MdDelete />
                                    </Button>

                                    <Box position="relative" display="flex" alignItems="center" w="80%" ml="auto">
                                        <Editable.Root
                                            defaultValue={item} fontWeight={"semibold"}
                                            fontSize={{lg: "sm", xl: "xl" }} 
                                            ml={{ xl: 1, base: 2 }}
                                            flex="1"
                                        >
                                            <Editable.Preview pointerEvents="none" />
                                            <Editable.Input />

                                            <Editable.Control>
                                                <Editable.EditTrigger asChild>
                                                    <IconButton p = {3}
                                                        position="absolute"
                                                        right={{sm: "8px", lg:"3px", xl:"1px"}}
                                                        variant="ghost"
                                                        size="xs"
                                                        aria-label="Edit"
                                                        color = "teal.200"
                                                    >
                                                        <LuPencilLine />
                                                    </IconButton>
                                                </Editable.EditTrigger>
                                            </Editable.Control>
                                        </Editable.Root>
                                    </Box>

                                </Box>
                            </GridItem>
                        ))
                    }
                </Grid>
            </Stack>
        </Box>
    );
}